/* Battery Widget */

import 'package:dimos/ui/core/themes/themes.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'dart:math';

// TODO: turn stateful widgets into stateless ones

class Battery extends StatelessWidget {
  const Battery({super.key});

  @override
  Widget build(BuildContext context) {
    final double soc = context.watch<StateOfCharge>().soc;
    final double maxSoc = context.watch<StateOfCharge>().maxSoc;

    return LayoutBuilder(
      builder: (context, constraints) {
        // Choose the smaller of width/height so it fits squarely
        final size = min(constraints.maxWidth, constraints.maxHeight);
        final stroke = size * 0.05; // stroke scales with size

        return Center(
          child: SizedBox(
            // w fix
            width: size/2,
            height: size/2,
            child: CircularProgressIndicator(
              value: soc / maxSoc,
              strokeWidth: stroke,
              backgroundColor: AppColors.whiteTransparent,
              color: soc > 20 ? const Color.fromARGB(255, 255, 255, 255) : Colors.red,
            ),
          ),
        );
      },
    );
  }
}
