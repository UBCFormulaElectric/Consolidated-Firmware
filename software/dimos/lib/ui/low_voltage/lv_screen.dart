/* LV screen */

import 'package:dimos/ui/low_voltage/shutdown_loop.dart';
import 'package:dimos/ui/notificationbar/notification_bar.dart';
import 'package:dimos/ui/racing/racing_screen.dart';
import 'package:flutter/material.dart';
import 'package:flutter_svg/flutter_svg.dart';

/* Main Widget */
class LVScreen extends StatelessWidget {

  final Map<String, String> _svgMap = {
    'logo': '../../../assets/fe_logo.svg'
  };

  LVScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        mainAxisSize: MainAxisSize.min,
        spacing: 50,
        children: [
          Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisSize: MainAxisSize.min,
            spacing: 50,
            children: [
              Center(
                child: Container( 
                  width: 200, 
                  height: 200,
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.all(Radius.circular(15)),
                    color: Colors.blue
                  )
                )
              ),
              Center(
                child: ConstrainedBox(
                  constraints: BoxConstraints(
                    minHeight: 200,
                    minWidth: 200,
                    maxHeight: 200,
                    maxWidth: 200
                  ),
                  child: SvgPicture.asset('assets/fe_logo.svg')
                )
              )
            ]
          ),
          ConstrainedBox(
              constraints: BoxConstraints(
                minHeight: 200,
                minWidth: 200,
                maxHeight: 200,
                maxWidth: 200,
              ),
              child: ShutdownLoop()
          ),
          ElevatedButton(
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => RacingScreen()),
              );
            },
            child: const Text('Speed'),
          )
        ]
      )
    );
  }
}

