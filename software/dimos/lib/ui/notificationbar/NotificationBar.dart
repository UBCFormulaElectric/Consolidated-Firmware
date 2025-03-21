import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';

class NotificationBar extends StatefulWidget implements PreferredSizeWidget {
  const NotificationBar({super.key});

  @override
  State<NotificationBar> createState() => _NotificationBarState();

  @override
  Size get preferredSize => const Size.fromHeight(100);
}

class _NotificationBarState extends State<NotificationBar> {

  @override
  Widget build(BuildContext context) {
    // switch to selector later
    return Consumer<WarningsList>(
      builder: (context, warningsList, child) {
        return Row(
          mainAxisAlignment: MainAxisAlignment.center,
          spacing: 10,
          children: warningsList.warnings.map(
            (item) => Text(
              item,
              style: const TextStyle(fontSize: 20, color: Colors.red),
            ),
          ).toList(),
        );
      },
    );
  }
}
