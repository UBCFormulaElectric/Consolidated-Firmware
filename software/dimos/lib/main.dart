import 'dart:io';
import 'package:dimos/src/app.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';

void main() async {
  setupWindow();

  runApp(const App());
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
