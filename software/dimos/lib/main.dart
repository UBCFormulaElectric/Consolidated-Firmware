import 'dart:io';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';
import 'package:dimos/ui/low_voltage/lv_screen.dart';
import 'package:dimos/ui/notificationbar/NotificationBar.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/dev_api.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await setupWindow();

  runApp(const App());
}

class App extends StatefulWidget {
  const App({super.key});

  @override
  State<App> createState() => _AppState();
}

class _AppState extends State<App> {
  late DevApiWorker _worker;
  late WarningsList _warningsList;

  @override
  void initState() {
    super.initState();
    _worker = DevApiWorker();
    _warningsList = WarningsList();

    // Start worker and listen for updates
    _worker.start((data) {
      _warningsList.updateList(data);
    });
  }

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => _warningsList,
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: AppTheme.darkTheme,
        home: Scaffold(
          appBar: NotificationBar(),
          body: Center(child: LVScreen()),
        ),
      ),
    );
  }
}

Future<void> setupWindow() async {
  await windowManager.ensureInitialized();
  windowManager.setTitleBarStyle(TitleBarStyle.normal);

  if (Platform.isWindows) {
    await WindowManager.instance.setMinimumSize(const Size(1024, 600));
    await WindowManager.instance.setMaximumSize(const Size(1024, 600));
  } else if (Platform.isLinux) {
    await WindowManager.instance.setMinimumSize(const Size(720, 480));
    await WindowManager.instance.setMaximumSize(const Size(720, 480));
  }
}
