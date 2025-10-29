/* Battery Widget */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';

class Battery extends StatefulWidget implements PreferredSizeWidget {
  const Battery({super.key});

  @override
  State<Battery> createState() => _BatteryState();

  @override
  Size get preferredSize => const Size.fromHeight(100);
}

class _BatteryState extends State<Battery> {

  @override
  Widget build(BuildContext context) {
    // switch to selector later
    return Consumer<StateOfCharge>(
      builder: (context, stateOfCharge, child) {
        return Center(
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              CircularProgressIndicator.adaptive(
                value: stateOfCharge.soc / stateOfCharge.maxSoc
              ),
            ],
          ),
        );
      },
    );
  }
}
