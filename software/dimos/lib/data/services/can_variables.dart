import 'package:provider/provider.dart';
import 'package:flutter/material.dart';

/* State Management */
class WarningsList extends ChangeNotifier {
  final List<String> _warnings = <String>["w", "fuck"];
  List<String> get warnings => _warnings.toList();

  void updateList(String data) {
    _warnings.add(data);
    notifyListeners();
  }
}