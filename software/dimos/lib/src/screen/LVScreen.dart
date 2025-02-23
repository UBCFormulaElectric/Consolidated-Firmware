// NotificationBar widget

import 'package:dimos/src/widgets/ShutdownLoop.dart';
import 'package:dimos/src/widgets/NotificationBar.dart';
import 'package:flutter/material.dart';
import 'package:flutter_svg/flutter_svg.dart';

class LVScreen extends StatelessWidget {
  final Map<String, String> svgMap = {
    'logo': '../../../assets/fe_logo.svg'
  };
  
  LVScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Container(
      child: SvgPicture.asset('assets/fe_logo.svg')
    );
  }
}

