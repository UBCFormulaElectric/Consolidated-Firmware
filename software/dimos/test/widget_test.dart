import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/input/debug_keys.dart';
import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/models/driver_controls.dart';
import 'package:dimos/data/models/info_feed.dart';
import 'package:dimos/data/models/snake_game.dart';
import 'package:dimos/routing/dashboard_controller.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/ui/dashboard_frame.dart';
import 'package:dimos/ui/debug/debug_panel.dart';

import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_signals.dart';
import 'package:dimos/data/services/vehicle_sim.dart';

void main() {
  test('sim snapshot: every board alive, loop closed, no faults', () {
    final vehicle = VehicleState()
      ..applySnapshot(VehicleSim().step(driving: false, regen: 0));
    expect(vehicle.hasFault, isFalse);
    expect(vehicle.system, SystemState.init);
    expect(vehicle.live, isFalse);
  });

  test('debug faults layer over CAN and clear', () {
    final vehicle = VehicleState()
      ..applySnapshot(VehicleSim().step(driving: false, regen: 0))
      ..openNode(ShutdownNodes.bspd);
    expect(vehicle.firstFaultNode, ShutdownNodes.bspd);
    expect(vehicle.system, SystemState.fault);
    vehicle.clearDebug();
    expect(vehicle.hasFault, isFalse);
  });

  test('system step reaches DRIVE and is blocked by faults', () {
    final vehicle = VehicleState()
      ..applySnapshot(VehicleSim().step(driving: false, regen: 0))
      ..systemStep()
      ..systemStep();
    expect(vehicle.system, SystemState.drive);
    expect(vehicle.live, isTrue);
    vehicle.pressEstop();
    expect(vehicle.estopOnly, isTrue);
    vehicle.systemStep();
    expect(vehicle.system, SystemState.fault);
  });

  test('the ring shows every car shutdown node, plus three without CAN status', () {
    expect(shutdownRing, hasLength(20));
    expect(shutdownRing.map((t) => t.node).whereType<int>().toSet(),
        {for (var i = 0; i < carShutdownLoop.length; i++) i});
    expect(shutdownRing.where((t) => t.node == null).map((t) => t.name),
        ['GLVMS', 'EFUSE 24V', 'AIR−']);
  });

  test('no CAN data means every board reports NO DATA', () {
    final vehicle = VehicleState();
    expect(vehicle.faultedBoards.map((b) => b.fault).toSet(), {'NO DATA'});
  });

  testWidgets('every page and the debug panel lay out without overflow', (tester) async {
    tester.view.physicalSize = const Size(1280, 800);
    tester.view.devicePixelRatio = 1;
    addTearDown(tester.view.reset);

    final vehicle = VehicleState()
      ..applySnapshot(VehicleSim().step(driving: false, regen: 0));
    final router = RouterProvider();
    final info = InfoFeed();
    final settings = DisplaySettings();
    final snake = SnakeGame();
    final controls = DriverControls(vehicle, router, info);
    final controller = DashboardController(
      vehicle: vehicle,
      router: router,
      controls: controls,
      info: info,
      settings: settings,
      snake: snake,
    );

    await tester.pumpWidget(MultiProvider(
      providers: [
        ChangeNotifierProvider.value(value: vehicle),
        ChangeNotifierProvider.value(value: controls),
        ChangeNotifierProvider.value(value: info),
        ChangeNotifierProvider.value(value: settings),
        ChangeNotifierProvider.value(value: FunEffects(vehicle, settings)),
        ChangeNotifierProvider.value(value: snake),
        ChangeNotifierProvider.value(value: router),
      ],
      child: MaterialApp(
        home: Column(children: [
          const Expanded(child: DashboardFrame()),
          DebugPanel(onInput: controller.inputs.route, onAction: controller.runDebugAction),
        ]),
      ),
    ));

    for (final action in [
      DebugAction.systemStep,
      DebugAction.systemStep,
      DebugAction.infoMessage,
      DebugAction.pressEstop,
      DebugAction.injectFault,
      DebugAction.theme,
    ]) {
      controller.runDebugAction(action);
      await tester.pump(const Duration(milliseconds: 100));
    }
    expect(router.isShowing(DashboardPages.faults), isTrue);

    controller.runDebugAction(DebugAction.clearFaults);
    controller.toggleSnake();
    await tester.pump(const Duration(milliseconds: 100));
    expect(snake.running, isTrue);

    // Let every timer (popups, info messages, effects) run out.
    snake.stop();
    await tester.pump(const Duration(seconds: 10));
  });
}
