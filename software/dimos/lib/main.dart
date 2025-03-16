import 'dart:io';
import 'package:dimos/ui/core/themes/themes.dart';
import 'package:dimos/ui/low_voltage/lv_screen.dart';
import 'package:dimos/ui/notificationbar/NotificationBar.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'package:provider/provider.dart';

void main() async {
  setupWindow();

  runApp(App());
}

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => WarningsList(),
      child: MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: AppTheme.darkTheme,
        home: Scaffold(
          appBar: NotificationBar(),
          body: Center(
            child: LVScreen()
          ),
        ),
      )
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
