/* Skidpad Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';
import 'package:flutter_svg/flutter_svg.dart';

class SkidpadScreen extends StatelessWidget {
  final String rotatedImage = 'assets/fe_logo.svg';

  const SkidpadScreen({super.key});

  @override
  Widget build(BuildContext context) {
    // watch for g's
    // layer circular speedometer on mock cartesian plane w/ vector
    // drop batter undernearh
    final int speed = context.watch<SpeedInteger>().speed;
    final double soc = context.watch<StateOfCharge>().soc;
    final double maxSoc = context.watch<StateOfCharge>().maxSoc;
    final double vecX = context.watch<SkidVector>().x;
    final double vecY = context.watch<SkidVector>().y;
    final double mag = context.watch<SkidVector>().mag;

    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          Transform(
            alignment: Alignment.center,
            transform: Matrix4(
              vecX/mag, -vecY/mag, 0, 0,
              vecY/mag,  vecX/mag, 0, 0,
              0,  0, 1, 0,
              0,  0, 0, 1,
            ),
            child: SvgPicture.asset(rotatedImage),
          ),
          Text(
            "${mag.toStringAsFixed(2)}G",
            style: const TextStyle(
              fontSize: 60,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
          ),
          Text(
            "${soc/maxSoc}%",
            style: const TextStyle(
              fontSize: 40,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
          )
        ],
      ),
    );
  }
}

