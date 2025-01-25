import 'dart:io';
import 'package:dimos/src/app.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';

void main() async {
  setupWindow();

  runApp(const Dimos());
}

void setupWindow() async {
  WidgetsFlutterBinding.ensureInitialized();
  await windowManager.ensureInitialized();
  if (Platform.isWindows) {
    WindowManager.instance.setMinimumSize(const Size(1024, 600));
    WindowManager.instance.setMaximumSize(const Size(1024, 600));
  }
}
