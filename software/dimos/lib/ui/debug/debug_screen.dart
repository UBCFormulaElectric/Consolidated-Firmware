/* Skidpad Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';
import 'package:dimos/ui/core/themes/themes.dart';

class DebugScreen extends StatelessWidget {
  const DebugScreen({super.key});

  @override
  Widget build(BuildContext context) {

    // might just watch for vars right now
    final List debugVars = context.watch<DebugVars>().debugVars;
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        spacing: 10,
        children: debugVars
          .map((s) => Text(s))
          .toList(),
      ),
    );
  }
}