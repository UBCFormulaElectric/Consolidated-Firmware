/* Racing Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';


class RacingScreen extends StatelessWidget {
  const RacingScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final int speed = context.watch<SpeedInteger>().speed;
    final double soc = context.watch<StateOfCharge>().soc;

    return Center(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          // --- SPEED ---
          _MetricBlock(
            value: "$speed",
            unit: "KPH",
          ),
          const SizedBox(width: 80), // spacing between speed and SOC
          // --- SOC ---
          _MetricBlock(
            value: "${soc.toStringAsFixed(0)}%",
            unit: "SOC",
          ),
        ],
      ),
    );
  }
}

class _MetricBlock extends StatelessWidget {
  final String value;
  final String unit;

  const _MetricBlock({
    required this.value,
    required this.unit,
  });

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        Text(
          value,
          style: const TextStyle(
            fontSize: 80,
            fontWeight: FontWeight.bold,
            color: Colors.white,
          ),
        ),
        const SizedBox(height: 8),
        Text(
          unit,
          style: const TextStyle(
            fontSize: 20,
            color: Colors.white70,
          ),
        ),
      ],
    );
  }
}