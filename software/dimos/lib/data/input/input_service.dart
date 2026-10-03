import 'dart:async';

import 'package:flutter/services.dart';

/// What the driver can ask the dashboard to do.
enum DashboardAction { nextPage, previousPage, select, back }

/// Driver input (buttons, rotary encoder) from the dashboard board.
///
/// STUB: the board's physical inputs aren't decided yet. The plan is for Linux to
/// expose them as key events (device-tree `gpio-keys` for buttons, `rotary-encoder`
/// for a knob), so they arrive here exactly like keyboard keys and a keyboard can
/// stand in during development. Update [_keyMap] once the board's keys are known.
class InputService {
  // Placeholder keys; replace with the keycodes the board's device tree emits.
  static final Map<LogicalKeyboardKey, DashboardAction> _keyMap = {
    LogicalKeyboardKey.arrowRight: DashboardAction.nextPage,
    LogicalKeyboardKey.arrowDown: DashboardAction.nextPage,
    LogicalKeyboardKey.arrowLeft: DashboardAction.previousPage,
    LogicalKeyboardKey.arrowUp: DashboardAction.previousPage,
    LogicalKeyboardKey.enter: DashboardAction.select,
    LogicalKeyboardKey.escape: DashboardAction.back,
  };

  final StreamController<DashboardAction> _actions = StreamController.broadcast();

  /// Driver actions, in the order they happen. Not wired to the UI yet.
  Stream<DashboardAction> get actions => _actions.stream;

  void start() => HardwareKeyboard.instance.addHandler(_onKey);

  bool _onKey(KeyEvent event) {
    if (event is! KeyDownEvent) return false;
    final action = _keyMap[event.logicalKey];
    if (action == null) return false;
    _actions.add(action);
    return true;
  }

  void dispose() {
    HardwareKeyboard.instance.removeHandler(_onKey);
    _actions.close();
  }
}
