import 'dart:io';
import 'package:dimos/src/widgets/Speedometer.dart';
import 'package:flutter/material.dart';
import 'package:window_manager/window_manager.dart';

// TODO: Stateful page + router

class Dimos extends StatelessWidget {
  const Dimos({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData(colorScheme: ColorScheme.dark()),
      home: Scaffold(
        body: Center(
          child: Speedometer(),
        ),
      ),
    );
  }
}