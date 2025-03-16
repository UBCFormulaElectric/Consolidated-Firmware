import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/dev_api.dart';

/* State Management */
class WarningsList extends ChangeNotifier {
  final List<String> _warnings = <String>["w", "fuck"];
  List<String> get warnings => _warnings.toList();

  void updateList(String data) {
    _warnings.add(data);
    notifyListeners();
  }
}

class NotificationBar extends StatefulWidget implements PreferredSizeWidget {
  const NotificationBar({super.key});

  @override
  State<NotificationBar> createState() => _NotificationBarState();

  @override
  Size get preferredSize => const Size.fromHeight(100);
}

class _NotificationBarState extends State<NotificationBar> {
  late DevApiWorker _worker;

  @override
  void initState() {
    super.initState();
    _worker = DevApiWorker();

    // Start worker and listen for updates
    _worker.start((data) {
      // Use Provider to update WarningsList
      Provider.of<WarningsList>(context, listen: false).updateList(data);
    });
  }

  @override
  Widget build(BuildContext context) {
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
