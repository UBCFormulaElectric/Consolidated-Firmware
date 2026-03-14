/* Braking Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';


class BrakingScreen extends StatelessWidget {
  const BrakingScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final double biasRatio = context.watch<BreakBias>().force/context.watch<BreakBias>().maxForce;
    // watch for break?

    // fill the entire screen properly
    return Scaffold(
      body: Stack(
        alignment: Alignment.center,
        children: [
          Positioned.fill(
            child: Row(
              children: [
                Expanded(
                  child: Container(color: Colors.black),
                ),
                Expanded(
                  child: Container(color: Colors.green),
                ),
              ],
            ),
          ),
          Positioned.fill(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children:[
                Text(
                  biasRatio.toStringAsFixed(2),
                  style: const TextStyle(
                    fontSize: 60,
                    fontWeight: FontWeight.bold,
                    color: Colors.white,
                  ),
                ),
                SizedBox(height:10),
                Text(
                  "Break Bias",
                  style: const TextStyle(
                    fontSize: 60,
                    fontWeight: FontWeight.bold,
                    color: Colors.white,
                  ),
                ),
              ]
            )
          )
        ],
      )
    );
  }
}