import 'dart:math';

import 'package:dimos/data/input/debug_keys.dart';
import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/models/driver_controls.dart';
import 'package:dimos/data/models/info_feed.dart';
import 'package:dimos/data/models/snake_game.dart';
import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_signals.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/routing/input_router.dart';

/// Wires the dashboard's models together: builds the wheel's input chain, reacts
/// to vehicle changes that move between pages, and runs development-only
/// [DebugAction]s.
class DashboardController {
  DashboardController({
    required this.vehicle,
    required this.router,
    required this.controls,
    required this.info,
    required this.settings,
    required this.snake,
  }) {
    vehicle.addListener(_onVehicleChanged);
  }

  final VehicleState vehicle;
  final RouterProvider router;
  final DriverControls controls;
  final InfoFeed info;
  final DisplaySettings settings;
  final SnakeGame snake;

  final Random _random = Random();
  int _lastFaultCount = 0;
  SystemState? _lastSystem;

  /// Wheel inputs go through these in order until one consumes them.
  late final InputRouter inputs = InputRouter([
    SnakeChord(toggleSnake),
    snake,
    controls,
    router,
  ]);

  void toggleSnake() {
    if (snake.running) return snake.stop();
    if (!settings.funFx) return;
    router.show(DashboardPages.drive);
    snake.start();
  }

  void runDebugAction(DebugAction action) {
    switch (action) {
      case DebugAction.systemStep:
        vehicle.systemStep();
      case DebugAction.injectFault:
        vehicle.injectFault();
      case DebugAction.pressEstop:
        vehicle.pressEstop();
      case DebugAction.faultImd:
        _debugOpen(ShutdownNodes.imd);
      case DebugAction.faultBspd:
        _debugOpen(ShutdownNodes.bspd);
      case DebugAction.faultBmsOk:
        _debugOpen(ShutdownNodes.bmsOk);
      case DebugAction.infoMessage:
        const pool = [
          ('BMS', 'CELL BALANCING'),
          ('VC', 'LOGGING ON'),
          ('VC', 'FAN HIGH'),
          ('RSM', 'MOTOR TEMP HIGH'),
          ('FSM', 'APPS PLAUSIBLE'),
          ('DAM', 'GPS LOCK'),
          ('BMS', 'CHARGE LIMIT'),
        ];
        final (source, message) = pool[_random.nextInt(pool.length)];
        info.add(source, message);
      case DebugAction.clearFaults:
        vehicle.clearDebug();
        router.show(DashboardPages.drive);
      case DebugAction.snake:
        toggleSnake();
      case DebugAction.funFx:
        snake.stop();
        settings.toggleFunFx();
      case DebugAction.theme:
        settings.toggleTheme();
      case DebugAction.togglePanel:
        settings.toggleDebugPanel();
    }
  }

  void _debugOpen(int node) {
    snake.stop();
    vehicle.openNode(node);
  }

  void _onVehicleChanged() {
    // A new fault takes the driver to the faults page.
    final faults = vehicle.faultCount;
    if (faults > _lastFaultCount) router.show(DashboardPages.faults);
    _lastFaultCount = faults;

    // Snake is only for sitting in INIT.
    final system = vehicle.system;
    if (system != _lastSystem && system != SystemState.init) snake.stop();
    _lastSystem = system;
  }

  void dispose() => vehicle.removeListener(_onVehicleChanged);
}
