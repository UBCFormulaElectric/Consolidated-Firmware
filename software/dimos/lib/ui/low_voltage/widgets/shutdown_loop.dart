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
    return Container(
      width: 200, 
      height: 200,
      decoration: BoxDecoration(
        borderRadius: BorderRadius.all(Radius.circular(15)),
        color: Colors.green
      )
    );
  }
}

