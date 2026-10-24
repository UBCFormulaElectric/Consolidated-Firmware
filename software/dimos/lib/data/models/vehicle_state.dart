import 'dart:async';
import 'dart:math';

import 'package:flutter/foundation.dart';

import 'package:dimos/data/services/can_signals.dart';

/// A board on the faults page: its vitals message and, if faulted, why.
class BoardStatus {
  const BoardStatus(this.board, this.vitalsId, this.fault);

  final Board board;
  final int? vitalsId;
  final String? fault;
}

/// The car as the dashboard shows it: the latest [CanSnapshot] (from CAN or
/// VehicleSim), with development-only debug faults and a forced system state
/// layered on top. Debug overrides stay until [clearDebug], even over live CAN.
class VehicleState extends ChangeNotifier {
  /// A board whose vitals are older than this is shown as timed out.
  static const int boardTimeoutMs = 500;
  static const Duration driveFlashDuration = Duration(milliseconds: 1400);

  final Random _random = Random();

  CanSnapshot? _can;
  SystemState? _forcedSystem;
  final Map<int, String> _debugNodeFaults = {};
  final Map<Board, String> _debugBoardFaults = {};

  bool _driveFlash = false;
  Timer? _driveFlashTimer;

  void applySnapshot(CanSnapshot snapshot) => _change(() => _can = snapshot);

  // ---- Values ----

  double get speedKph => _can?.speedKph ?? 0;
  double get socPercent => (_can?.socPercent ?? 0).toDouble();
  double get throttlePercent => (_can?.throttlePercent ?? 0).clamp(0, 100);
  double get brakePercent =>
      ((_can?.frontBrakePsi ?? 0) / brakeFullScalePsi * 100).clamp(0, 100);

  /// Tractive power as a percent of [maxTractivePowerKw]; negative is regen.
  double get powerPercent => (_can?.tsPowerKw ?? 0) / maxTractivePowerKw * 100;

  int get canLoggingRemainingErrors => _can?.canLoggingRemainingErrors ?? 0;

  /// True for the moment the car goes from READY TO DRIVE into DRIVE.
  bool get driveFlash => _driveFlash;

  // ---- System state ----

  bool _alive(Board board) {
    final age = _can?.ageOf(board);
    return age != null && age <= boardTimeoutMs;
  }

  SystemState get _baseSystem =>
      _forcedSystem ??
      (_alive(Board.vc) ? systemStateFromVc(_can!.vcState) : SystemState.init);

  /// What the state bar shows: any fault wins over the VC's state.
  SystemState get system => hasFault ? SystemState.fault : _baseSystem;

  /// Driver settings (TV, regen, drive mode) can only change when this is true.
  bool get live =>
      system == SystemState.readyToDrive || system == SystemState.drive;

  // ---- Faults ----

  /// The first open shutdown loop node (index into [carShutdownLoop]), if any.
  /// The VC's view of the loop wins; without the VC, each node's own status
  /// from the board that reports it.
  int? get firstFaultNode {
    final can = _can;
    int? fromCan;
    if (_alive(Board.vc)) {
      fromCan = can!.firstFaultNode;
    } else if (can != null) {
      for (final (i, node) in carShutdownLoop.indexed) {
        if (_alive(node.reportedBy) && !can.shutdownNodeOk[i]) {
          fromCan = i;
          break;
        }
      }
    }
    final fromDebug = _debugNodeFaults.keys.fold<int?>(
        null, (first, i) => first == null ? i : min(first, i));
    if (fromCan == null) return fromDebug;
    if (fromDebug == null) return fromCan;
    return min(fromCan, fromDebug);
  }

  List<BoardStatus> get boards => [
        for (final board in Board.values)
          BoardStatus(board, _can?.boardVitalsId[board.index],
              _debugBoardFaults[board] ?? _canBoardFault(board)),
      ];

  String? _canBoardFault(Board board) {
    final age = _can?.ageOf(board);
    if (age == null) return 'NO DATA';
    if (age > boardTimeoutMs) return 'CAN TIMEOUT';
    if (board == Board.bms && isBmsFaultState(_can!.bmsState)) return 'BMS FAULT';
    if (board == Board.vc && systemStateFromVc(_can!.vcState) == SystemState.fault) {
      return 'VC FAULT';
    }
    return null;
  }

  List<BoardStatus> get faultedBoards =>
      boards.where((b) => b.fault != null).toList();

  /// The open loop counts once, however many nodes are open after it.
  int get faultCount => (firstFaultNode != null ? 1 : 0) + faultedBoards.length;

  bool get hasFault => faultCount > 0;

  /// The loop is only open at an e-stop, so the driver just needs to release it.
  bool get estopOnly {
    final first = firstFaultNode;
    return first != null && carShutdownLoop[first].isEstop && faultedBoards.isEmpty;
  }

  // ---- Debug overrides ----

  /// INIT → READY TO DRIVE → DRIVE → INIT; does nothing while faulted.
  void systemStep() {
    if (hasFault) return;
    _change(() => _forcedSystem = switch (_baseSystem) {
          SystemState.init || SystemState.fault => SystemState.readyToDrive,
          SystemState.readyToDrive => SystemState.drive,
          SystemState.drive => SystemState.init,
        });
  }

  /// Faults a random healthy board, or opens a random node before the current
  /// first fault.
  void injectFault() {
    const boardFaults = ['CAN TIMEOUT', 'CELL OVERTEMP', 'OVERCURRENT', 'UNDERVOLTAGE'];
    final freeBoards = boards.where((b) => b.fault == null).map((b) => b.board).toList();
    final freeNodes = List.generate(firstFaultNode ?? carShutdownLoop.length, (i) => i);
    if (freeBoards.isEmpty && freeNodes.isEmpty) return;

    _change(() {
      if (freeBoards.isNotEmpty && (freeNodes.isEmpty || _random.nextDouble() < 0.3)) {
        _debugBoardFaults[freeBoards[_random.nextInt(freeBoards.length)]] =
            boardFaults[_random.nextInt(boardFaults.length)];
      } else {
        _debugNodeFaults
          ..clear()
          ..[freeNodes[_random.nextInt(freeNodes.length)]] = 'LOOP OPEN';
      }
      _forcedSystem = SystemState.init;
    });
  }

  void openNode(int node) => _change(() {
        _debugNodeFaults
          ..clear()
          ..[node] = 'LOOP OPEN';
        _forcedSystem = SystemState.init;
      });

  void pressEstop() {
    final free = [
      for (final (i, node) in carShutdownLoop.indexed)
        if (node.isEstop && !_debugNodeFaults.containsKey(i)) i,
    ];
    if (free.isEmpty) return;
    _change(() {
      _debugNodeFaults[free[_random.nextInt(free.length)]] = 'PRESSED';
      _forcedSystem = SystemState.init;
    });
  }

  void clearDebug() => _change(() {
        _debugNodeFaults.clear();
        _debugBoardFaults.clear();
        _forcedSystem = null;
      });

  void _change(void Function() update) {
    final before = system;
    update();
    if (before == SystemState.readyToDrive && system == SystemState.drive) {
      _driveFlash = true;
      _driveFlashTimer?.cancel();
      _driveFlashTimer = Timer(driveFlashDuration, () {
        _driveFlash = false;
        notifyListeners();
      });
    }
    notifyListeners();
  }

  @override
  void dispose() {
    _driveFlashTimer?.cancel();
    super.dispose();
  }
}
