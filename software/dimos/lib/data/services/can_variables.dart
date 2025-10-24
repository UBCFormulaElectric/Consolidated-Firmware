import 'package:provider/provider.dart';
import 'package:flutter/material.dart';

/// State Management
/// File to hold all the associated variables with widgets
/// Current format is mid but defintely can benefit from
/// Abstraction which is a future task
/// Because we can see that every variable will either
/// Be a string/integer/list
/// And will only have two functions
/// Which only need the "name" of the respective JSON can call to update their values

/* CAN Warnings */
class WarningsList extends ChangeNotifier {
  final List<String> _warnings = <String>["hey"];
  List<String> get warnings => _warnings.toList();

  void updateListDev(String data) {
    _warnings.add(data);
    notifyListeners();
  }

  void updateListCan() {
    // for now triggers on recieving frames, just put dummy
    _warnings.add("TRIGGERED");
  }
}

/* Speed */
class SpeedInteger extends ChangeNotifier {
  int _speed = 0;
  int get speed => _speed;

  void updateListDev(String data) {
    _speed += 1;
    notifyListeners();
  }

  void updateListCan() {
    // JSON CAN calls + maybe mutex??
    // then ONLY notify if changes are made!!
    _speed += 1;
  }
}