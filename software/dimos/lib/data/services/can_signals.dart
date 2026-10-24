/// What the dashboard knows about the car, independent of where it came from
/// (jsoncan over CAN, or VehicleSim). Values mirror `DimosSignals` in
/// native/jsoncan_bridge; enum values come from the car's CAN JSON
/// (can_bus/hexray/*_enum.json).
library;

/// The boards whose vitals the dashboard watches, in `DimosSignals.board_*` order.
enum Board {
  bms('BMS', 'ACCUMULATOR'),
  vc('VC', 'VEHICLE CONTROL'),
  fsm('FSM', 'FRONT SENSORS'),
  rsm('RSM', 'REAR SENSORS'),
  crit('CRIT', 'COCKPIT'),
  dam('DAM', 'DATA ACQUISITION');

  const Board(this.label, this.description);

  final String label;
  final String description;
}

/// One node of the shutdown loop and the board that reports its status.
class ShutdownNode {
  const ShutdownNode(this.name, this.reportedBy, {this.isEstop = false});

  final String name;
  final Board reportedBy;
  final bool isEstop;
}

/// The shutdown loop in loop order; index i is `ShutdownNode` value i in
/// shared_enum.json, and `DimosSignals.shutdown_node_ok[i]`.
const List<ShutdownNode> shutdownLoop = [
  ShutdownNode('BMS OK', Board.bms),
  ShutdownNode('IMD', Board.bms),
  ShutdownNode('BSPD', Board.bms),
  ShutdownNode('HV+ ILCK', Board.bms),
  ShutdownNode('HV− ILCK', Board.bms),
  ShutdownNode('MSD/EMETER', Board.vc),
  ShutdownNode('RL ILCK', Board.rsm),
  ShutdownNode('RR ILCK', Board.vc),
  ShutdownNode('R ESTOP', Board.dam, isEstop: true),
  ShutdownNode('L ESTOP', Board.dam, isEstop: true),
  ShutdownNode('COCKPIT', Board.fsm, isEstop: true),
  ShutdownNode('BOTS', Board.fsm),
  ShutdownNode('FR ILCK', Board.fsm),
  ShutdownNode('FL ILCK', Board.fsm),
  ShutdownNode('INERTIA', Board.vc),
  ShutdownNode('TSMS', Board.vc),
];

/// Indices into [shutdownLoop] that dashboard debug faults refer to.
abstract final class ShutdownNodes {
  static const int bmsOk = 0;
  static const int imd = 1;
  static const int bspd = 2;
}

/// VC_State (VCState in VC_enum.json), collapsed to what the driver needs.
enum SystemState { init, readyToDrive, drive, fault }

SystemState systemStateFromVc(int vcState) => switch (vcState) {
      5 => SystemState.readyToDrive, // VC_HV_ON_STATE
      6 || 7 => SystemState.drive, // VC_DRIVE_STATE, VC_DRIVE_WARNING_STATE
      8 || 9 => SystemState.fault, // VC_FAULT_STATE, VC_INVERTER_FAULT_HANDELER
      _ => SystemState.init,
    };

/// BmsState values (BMS_enum.json) the dashboard reports as a BMS fault.
bool isBmsFaultState(int bmsState) =>
    bmsState == 7 || bmsState == 9; // BMS_FAULT_STATE, BMS_CHARGE_FAULT_STATE

/// Full scale of the power bar: the FSAE tractive power limit.
const double maxTractivePowerKw = 80;

/// PLACEHOLDER: front brake pressure shown as a full brake bar, until the
/// team settles on what full braking reads on the FSM.
const int brakeFullScalePsi = 1000;

/// One reading of everything the dashboard shows from the car.
class CanSnapshot {
  const CanSnapshot({
    required this.speedKph,
    required this.tsPowerKw,
    required this.throttlePercent,
    required this.frontBrakePsi,
    required this.socPercent,
    required this.vcState,
    required this.bmsState,
    required this.canLoggingRemainingErrors,
    required this.firstFaultNode,
    required this.shutdownNodeOk,
    required this.boardAgeMs,
    required this.boardVitalsId,
  });

  final double speedKph;
  final double tsPowerKw;
  final double throttlePercent;
  final int frontBrakePsi;
  final int socPercent;
  final int vcState;
  final int bmsState;
  final int canLoggingRemainingErrors;

  /// Index into [shutdownLoop], or null when the loop is closed.
  final int? firstFaultNode;

  /// Indexed like [shutdownLoop].
  final List<bool> shutdownNodeOk;

  /// Since each [Board]'s vitals were last received, or null if never.
  final List<int?> boardAgeMs;
  final List<int> boardVitalsId;

  int? ageOf(Board board) => boardAgeMs[board.index];
}
