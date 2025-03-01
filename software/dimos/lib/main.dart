import 'dart:io';
import 'package:dimos/ui/low_voltage/widgets/lv_screen.dart';
import 'package:dimos/ui/notificationbar/widgets/NotificationBar.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';

void main() async {
  setupWindow();

  runApp(const App());
}

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: new ThemeData(scaffoldBackgroundColor: const Color.fromARGB(255, 0, 0, 0)),
      home: Scaffold(
        appBar: NotificationBar(),
        body: Center(
          child: LVScreen()
        ),
      ),
    );
  }
}

void setupWindow() async {
  WidgetsFlutterBinding.ensureInitialized();
  await windowManager.ensureInitialized();
  windowManager.setTitleBarStyle(TitleBarStyle.normal);
  if (Platform.isWindows) {
    WindowManager.instance.setMinimumSize(const Size(1024, 600));
    WindowManager.instance.setMaximumSize(const Size(1024, 600));
  } else if (Platform.isLinux) {
    WindowManager.instance.setMinimumSize(const Size(720, 480));
    WindowManager.instance.setMaximumSize(const Size(720, 480));
  }
}
