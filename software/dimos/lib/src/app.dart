import 'dart:io';
import 'package:dimos/src/widgets/Speedometer.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';
import 'screen/LVScreen.dart';
import 'widgets/NotificationBar.dart';

// TODO: Stateful page + router 

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