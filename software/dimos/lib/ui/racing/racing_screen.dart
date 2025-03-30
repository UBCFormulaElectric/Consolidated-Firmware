/* Racing Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';

class RacingScreen extends StatelessWidget{

  @override
  Widget build(BuildContext context) {
    // switch to selector later
    return Consumer<SpeedInteger>(
      builder: (context, speedInteger, child) {
        return Center(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisSize: MainAxisSize.min,
            spacing: 50,
            children: [ 
              Text(
                speedInteger.speed.toString(),
                style: Theme.of(context).textTheme.bodyLarge
              ),
              // ElevatedButton(
              //   onPressed: () {
              //     Navigator.pop(context);
              //   },
              //   child: const Text('Go back to LV'),
              // )
            ]
          )
        );
      },
    );
  }
}
