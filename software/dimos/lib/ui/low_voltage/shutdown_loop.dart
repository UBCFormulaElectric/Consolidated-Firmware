// ShutdownLoop Widget

import 'package:flutter/material.dart';

class ShutdownLoop extends StatelessWidget {
  // a basic solution to this is to make various svg images
  // and transition through them
  // a more mathmatical soluton is to define a node class
  // which takes in absolute solution
  // and is able to animate itself moving towards anohter passed in node
  // suppose we use the easier solution for now

  // test with mock on shit svgs
  // mock should just push latest each time

  final Map node_to_svg = {
    "BSPD": "svg1",
    "IMD": "svg2",
    "BMS": "svg3",
    "EStop": "svg4",
    "Inertia": "svg5",
    "BOTS": "svg6",
    "LEStop": "svg7",
    "REStop": "svg8",
    "TSMS": "svg9",
    "PCM": "svg10",
    "HVD": "svg11"
  };

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

