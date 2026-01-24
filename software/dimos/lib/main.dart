/* Main App */

import 'dart:io';
import 'package:dimos/data/services/can_api.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/ui/core/themes/themes.dart';
import 'package:dimos/ui/notificationbar/notification_bar.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/dev_api.dart';

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

// pretty sure provider passing all the way the down
// and also does not change depending on which root you pass it down
// alsoing initing all the listeners and shit is optimal here

class _AppState extends State<App> {
  late DevApiWorker _devWorker;
  late CanApiWorker _canWorker;
  late SpeedInteger _speedInteger;
  late WarningsList _warningsList;
  late StateOfCharge _stateOfCharge;
  late ShutdownLoopNodes _shutdownLoopNodes;
  late SkidVector _skidVector;
  late BreakBias _breakBias;
  late DebugVars _debugVars;

  @override
  void initState() {
    super.initState();
    _warningsList = WarningsList();
    _speedInteger = SpeedInteger();
    _stateOfCharge = StateOfCharge();
    _shutdownLoopNodes = ShutdownLoopNodes();
    _skidVector = SkidVector();
    _breakBias = BreakBias();
    _debugVars = DebugVars();

    if (Platform.isWindows || Platform.isMacOS) {
      // have some basic dev api setup to introduce can
      _devWorker = DevApiWorker();
      _devWorker.start((data) {
        _warningsList.updateListDev(data);
        _speedInteger.updateVarDev(data);
        _stateOfCharge.updateVarDev(data);
        _shutdownLoopNodes.updateVarDev(data);
        _skidVector.updateVarDev(data);
        _breakBias.updateVarDev(data);
        _debugVars.updateVarDev(data);
      });
    } else if (Platform.isLinux) {
      _canWorker = CanApiWorker();
      _canWorker.start((data) {
        _warningsList.updateListCan();
        _speedInteger.updateVarCan();
        _stateOfCharge.updateVarCan();
        _shutdownLoopNodes.updateVarCan();
        _skidVector.updateVarCan();
        _breakBias.updateVarCan();
        _debugVars.updateVarCan();
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => _warningsList),
        ChangeNotifierProvider(create: (_) => _speedInteger),
        ChangeNotifierProvider(create: (_) => _stateOfCharge),
        ChangeNotifierProvider(create: (_) => _shutdownLoopNodes),
        ChangeNotifierProvider(create: (_) => _skidVector),
        ChangeNotifierProvider(create: (_) => _breakBias),
        ChangeNotifierProvider(create: (_) => _debugVars),
        ChangeNotifierProvider(create: (_) => RouterProvider())
      ],
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: AppTheme.darkTheme,
        home: Scaffold(
          appBar: NotificationBar(),
          body: Center(
            child: Row(
              spacing: 10,
              children: [
                  Expanded(flex: 6, child: Consumer<RouterProvider>(
                    builder: (context, navigation, _) => navigation.currentPage,
                  )),
                  Expanded(flex: 2, child: RouterPanel())
              ]
            )
          )
        )
      )
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
