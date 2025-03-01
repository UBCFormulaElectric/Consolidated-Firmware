// ShutdownLoop Widget

import 'package:flutter/material.dart';

class ShutdownLoop extends StatelessWidget {
  // For now what if I just do x^2 + y^2 = r
  // and drop a few points on a circle
  // do this AFTER can works 
  // for now just stick a circle LOL

  ShutdownLoop({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Container(
          width: 100.0,
          height: 100.0,
          decoration: BoxDecoration(
            shape: BoxShape.circle,
            border: Border.all(
              color: const Color(0x36FB61),
              width: 5.0,
            ),
          ),
        )
      ),
    );
  }
}

