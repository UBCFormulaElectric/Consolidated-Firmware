import 'dart:async';

import 'package:flutter/foundation.dart';

import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_signals.dart';

/// How the dashboard looks: day/night palette, fun effects, and (development
/// only) whether the debug panel is showing.
class DisplaySettings extends ChangeNotifier {
  bool _night = true;
  bool _funFx = true;
  bool _debugPanel = false;

  bool get night => _night;
  bool get funFx => _funFx;
  bool get debugPanel => _debugPanel;

  void toggleTheme() {
    _night = !_night;
    notifyListeners();
  }

  void toggleFunFx() {
    _funFx = !_funFx;
    notifyListeners();
  }

  void toggleDebugPanel() {
    _debugPanel = !_debugPanel;
    notifyListeners();
  }
}

/// Easter eggs, driven by the vehicle's faults (off with [DisplaySettings.funFx]):
/// a "screenshot" when the IMD, BSPD or BMS OK node opens the loop, and an
/// achievement for faulting every board with the loop open at its first node.
class FunEffects extends ChangeNotifier {
  FunEffects(this._vehicle, this._settings) {
    _vehicle.addListener(_onVehicleChanged);
    _settings.addListener(_onSettingsChanged);
  }

  static const Duration shotDuration = Duration(milliseconds: 4100);
  static const Duration achievementDuration = Duration(seconds: 5);
  static const Set<int> _shotNodes = {
    ShutdownNodes.imd,
    ShutdownNodes.bspd,
    ShutdownNodes.bmsOk,
  };

  final VehicleState _vehicle;
  final DisplaySettings _settings;

  int? _shotNode;
  bool _achievement = false;
  int? _lastFirstFault;
  bool _wasParty = false;
  Timer? _shotTimer;
  Timer? _achievementTimer;

  /// The shutdown node whose "screenshot" is animating, if any.
  int? get shotNode => _shotNode;
  bool get achievement => _achievement;

  /// Every board faulted with the loop open at its very first node.
  bool get party =>
      _settings.funFx &&
      _vehicle.firstFaultNode == 0 &&
      _vehicle.faultedBoards.length == Board.values.length;

  void _onVehicleChanged() {
    final first = _vehicle.firstFaultNode;
    if (first != _lastFirstFault && first != null && _settings.funFx &&
        _shotNodes.contains(first)) {
      _shotNode = first;
      _shotTimer?.cancel();
      _shotTimer = Timer(shotDuration, () {
        _shotNode = null;
        notifyListeners();
      });
      notifyListeners();
    }
    _lastFirstFault = first;

    final isParty = party;
    if (isParty && !_wasParty) {
      _achievement = true;
      _achievementTimer?.cancel();
      _achievementTimer = Timer(achievementDuration, () {
        _achievement = false;
        notifyListeners();
      });
      notifyListeners();
    }
    _wasParty = isParty;
  }

  void _onSettingsChanged() {
    if (_settings.funFx) return;
    _shotTimer?.cancel();
    _achievementTimer?.cancel();
    _shotNode = null;
    _achievement = false;
    notifyListeners();
  }

  @override
  void dispose() {
    _vehicle.removeListener(_onVehicleChanged);
    _settings.removeListener(_onSettingsChanged);
    _shotTimer?.cancel();
    _achievementTimer?.cancel();
    super.dispose();
  }
}
