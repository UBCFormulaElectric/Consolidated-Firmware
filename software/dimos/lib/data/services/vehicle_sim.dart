import 'dart:math';

import 'package:dimos/data/services/can_signals.dart';

/// Fake car for working on the UI without CAN (`DIMOS_CAN_SOURCE=sim`, and
/// always on Windows where libdimos_jsoncan isn't built). Laps a made-up track
/// while the dashboard is in DRIVE; every board is alive and the shutdown loop
/// is closed, so faults only come from the debug keys.
class VehicleSim {
  static const Duration period = Duration(milliseconds: 60);

  final Random _random = Random();
  double _t = 0;
  double _speed = 0;
  double _soc = 82;

  /// Advances one [period]. [regen] (0-1) is how hard braking feeds back.
  CanSnapshot step({required bool driving, required double regen}) {
    _t += period.inMilliseconds / 1000;
    final phase = (sin(_t * 0.42) + sin(_t * 0.17 + 1.3)) / 2;
    final throttle = driving ? max(0.0, phase) * 96 + _random.nextDouble() * 3 : 0.0;
    final brake = driving ? max(0.0, -phase) * 88 + _random.nextDouble() * 2 : 0.0;
    _speed += driving ? throttle * 0.06 - brake * 0.09 - 0.25 : -_speed * 0.08;
    _speed = _speed.clamp(0, 118);
    final power = driving ? throttle * 0.95 - brake * regen : 0.0;
    _soc = max(0, _soc - (driving ? max(0, power) * 0.00018 : 0));

    return CanSnapshot(
      speedKph: _speed,
      // The sim works in percent of full power; the dashboard expects kW.
      tsPowerKw: power / 100 * maxTractivePowerKw,
      throttlePercent: throttle,
      frontBrakePsi: (brake / 100 * brakeFullScalePsi).round(),
      socPercent: _soc.round(),
      vcState: 0,
      bmsState: 0,
      canLoggingRemainingErrors: 0,
      firstFaultNode: null,
      shutdownNodeOk: List.filled(shutdownLoop.length, true),
      boardAgeMs: List.filled(Board.values.length, 0),
      boardVitalsId: [for (final board in Board.values) _vitalsIds[board]!],
    );
  }

  // The CAN JSON's vitals message IDs, for the faults page.
  static const Map<Board, int> _vitalsIds = {
    Board.bms: 400,
    Board.vc: 500,
    Board.fsm: 600,
    Board.rsm: 700,
    Board.crit: 900,
    Board.dam: 800,
  };
}

