// NotificationBar widget

import 'package:flutter/material.dart';
import 'package:flutter_svg/flutter_svg.dart';

class NotificationBar extends StatefulWidget implements PreferredSizeWidget {
  const NotificationBar({super.key});

  @override
  State<NotificationBar> createState() => _NotificationBar();

  @override
  Size get preferredSize => const Size.fromHeight(100);
}

class _NotificationBar extends State<NotificationBar> {
  List<String> _warnings = ["W", "W"];

  // ofc hook up to CANtable later
  void addWarning() {
    setState(() { _warnings.add("W"); });
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: _warnings.map((item) => Text(
        item,
        style: const TextStyle(
        fontSize: 20,
        color: Colors.red))).toList(),
      );
  }
}
