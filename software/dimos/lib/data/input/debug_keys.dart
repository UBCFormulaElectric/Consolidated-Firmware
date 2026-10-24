import 'dart:async';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

/// Development-only keys and the debug panel: on in debug builds, or in any
/// build with `--dart-define=DIMOS_DEV_KEYS=true`. Never on the car otherwise.
const bool devKeysEnabled =
    kDebugMode || bool.fromEnvironment('DIMOS_DEV_KEYS');

/// Things a developer can do to the dashboard that the car can't (yet).
enum DebugAction {
  systemStep('TS / DRIVE STEP', LogicalKeyboardKey.keyS),
  injectFault('INJECT FAULT', LogicalKeyboardKey.keyF),
  pressEstop('PRESS ESTOP', LogicalKeyboardKey.keyX),
  faultImd('DBG IMD', LogicalKeyboardKey.keyI),
  faultBspd('DBG BSPD', LogicalKeyboardKey.keyB),
  faultBmsOk('DBG BMS OK', LogicalKeyboardKey.keyM),
  infoMessage('INFO MSG', LogicalKeyboardKey.keyN),
  clearFaults('CLEAR FAULTS', LogicalKeyboardKey.keyC),
  snake('SNAKE', LogicalKeyboardKey.keyK),
  funFx('FUN FX', LogicalKeyboardKey.keyU),
  theme('DAY / NIGHT', LogicalKeyboardKey.keyL),
  togglePanel('DEBUG PANEL', LogicalKeyboardKey.backquote);

  const DebugAction(this.label, this.key);

  final String label;
  final LogicalKeyboardKey key;
}

/// Turns development-only key presses into [DebugAction]s. Kept apart from
/// InputService, which only stands in for the wheel's real inputs.
///
/// | Key | Action |
/// |---|---|
/// | S | Step the system state: INIT → READY TO DRIVE → DRIVE → INIT |
/// | F | Open a random shutdown node or fault a random board |
/// | X | Press a random e-stop |
/// | I / B / M | Open the IMD / BSPD / BMS OK shutdown node |
/// | N | Show a random info message |
/// | C | Clear debug faults and the forced system state |
/// | K | Snake (also: LEFT + RIGHT + OK together) |
/// | U | Fun effects on / off |
/// | L | Day / night theme |
/// | ` | Show / hide the debug panel |
class DebugKeyService {
  static final Map<LogicalKeyboardKey, DebugAction> _keys = {
    for (final action in DebugAction.values) action.key: action,
  };

  final StreamController<DebugAction> _actions = StreamController.broadcast();

  Stream<DebugAction> get actions => _actions.stream;

  void start() {
    if (devKeysEnabled) HardwareKeyboard.instance.addHandler(_onKey);
  }

  /// Runs [action] as if its key was pressed (debug panel buttons).
  void emit(DebugAction action) => _actions.add(action);

  bool _onKey(KeyEvent event) {
    final action = _keys[event.logicalKey];
    if (action == null) return false;
    if (event is KeyDownEvent) _actions.add(action);
    return true;
  }

  void dispose() {
    HardwareKeyboard.instance.removeHandler(_onKey);
    _actions.close();
  }
}
