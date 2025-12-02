/* Braking Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';


class BrakingScreen extends StatelessWidget {
  const BrakingScreen({super.key});

  @override
  Widget build(BuildContext context) {
    // watch for break?

    return Center(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          Text("Breaking"),
        ],
      ),
    );
  }
}