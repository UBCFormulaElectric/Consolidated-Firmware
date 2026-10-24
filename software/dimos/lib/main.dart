/* Main App */

import 'dart:async';
import 'dart:io';
import 'package:dimos/data/input/debug_keys.dart';
import 'package:dimos/data/input/input_service.dart';
import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/models/driver_controls.dart';
import 'package:dimos/data/models/info_feed.dart';
import 'package:dimos/data/models/snake_game.dart';
import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_service.dart';
import 'package:dimos/data/services/can_signals.dart';
import 'package:dimos/data/services/vehicle_sim.dart';
import 'package:dimos/routing/dashboard_controller.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/ui/core/themes/themes.dart';
import 'package:dimos/ui/dashboard_frame.dart';
import 'package:dimos/ui/debug/debug_panel.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:provider/provider.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  // await setupWindow();

  runApp(const App());
}

class App extends StatefulWidget {
  const App({super.key});

  @override
  State<App> createState() => _AppState();
}

// Data flows one way: CanService (or VehicleSim) → VehicleState → UI. Wheel
// inputs go InputService → DashboardController.inputs (the middleware chain) →
// the models; debug keys go DebugKeyService → DashboardController.
class _AppState extends State<App> {
  final CanConfig _canConfig = CanConfig.fromEnvironment();
  CanService? _can;
  Timer? _simTimer;

  final VehicleState _vehicle = VehicleState();
  final InfoFeed _info = InfoFeed();
  final DisplaySettings _settings = DisplaySettings();
  final RouterProvider _router = RouterProvider();
  final SnakeGame _snake = SnakeGame();
  late final DriverControls _controls = DriverControls(_vehicle, _router, _info);
  late final FunEffects _fun = FunEffects(_vehicle, _settings);
  late final DashboardController _controller = DashboardController(
    vehicle: _vehicle,
    router: _router,
    controls: _controls,
    info: _info,
    settings: _settings,
    snake: _snake,
  );

  final InputService _input = InputService();
  final DebugKeyService _debugKeys = DebugKeyService();
  late StreamSubscription<DashboardInput> _inputSubscription;
  late StreamSubscription<DebugAction> _debugSubscription;

  @override
  void initState() {
    super.initState();
    _inputSubscription = _input.inputs.listen(_controller.inputs.route);
    _input.start();
    _debugSubscription = _debugKeys.actions.listen(_controller.runDebugAction);
    _debugKeys.start();

    if (_canConfig.simulated) {
      final sim = VehicleSim();
      _simTimer = Timer.periodic(VehicleSim.period, (_) {
        _vehicle.applySnapshot(sim.step(
          driving: _vehicle.system == SystemState.drive,
          regen: _controls.regenStrength,
        ));
      });
    } else {
      final can = CanService(config: _canConfig);
      can.errors.addListener(() {
        for (final error in can.errors.value) {
          _info.setWarning('CAN: $error', true, source: 'CAN', message: error.toUpperCase());
        }
      });
      can.addListener(_onCanUpdate);
      can.start();
      _can = can;
    }
  }

  // Called at most every CanService.refreshPeriod, and only when new frames
  // were decoded.
  void _onCanUpdate() {
    final snapshot = _can!.jsoncan.readSignals();
    _vehicle.applySnapshot(snapshot);
    final remainingErrors = snapshot.canLoggingRemainingErrors;
    _info.setWarning('CAN_LOG_ERRORS', remainingErrors > 0,
        source: 'VC', message: 'CAN LOG ERRORS $remainingErrors');
  }

  @override
  void dispose() {
    _inputSubscription.cancel();
    _debugSubscription.cancel();
    _input.dispose();
    _debugKeys.dispose();
    _simTimer?.cancel();
    _can?.dispose();
    _controller.dispose();
    _controls.dispose();
    _fun.dispose();
    for (final model in [_vehicle, _info, _settings, _router, _snake]) {
      model.dispose();
    }
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider.value(value: _vehicle),
        ChangeNotifierProvider.value(value: _controls),
        ChangeNotifierProvider.value(value: _info),
        ChangeNotifierProvider.value(value: _settings),
        ChangeNotifierProvider.value(value: _fun),
        ChangeNotifierProvider.value(value: _snake),
        ChangeNotifierProvider.value(value: _router),
      ],
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: AppTheme.darkTheme,
        home: Scaffold(
          backgroundColor: Colors.black,
          body: Column(
            children: [
              const Expanded(child: Center(child: DashboardFrame())),
              if (devKeysEnabled)
                Selector<DisplaySettings, bool>(
                  selector: (_, settings) => settings.debugPanel,
                  builder: (context, show, child) => show
                      ? DebugPanel(onInput: _input.emit, onAction: _debugKeys.emit)
                      : const SizedBox.shrink(),
                ),
            ],
          ),
        ),
      ),
    );
  }
}

Future<void> setupWindow() async {
  // why is this dying on linux, i think some of these functions deadass dont work on linux
  await windowManager.ensureInitialized();
  windowManager.setTitleBarStyle(TitleBarStyle.normal);

  if (Platform.isWindows || Platform.isMacOS) {
    await WindowManager.instance.setMinimumSize(const Size(1024, 600));
    await WindowManager.instance.setMaximumSize(const Size(1024, 600));
  } else if (Platform.isLinux) {
    await WindowManager.instance.setMinimumSize(const Size(720, 480));
    await WindowManager.instance.setMaximumSize(const Size(720, 480));
  }
}
