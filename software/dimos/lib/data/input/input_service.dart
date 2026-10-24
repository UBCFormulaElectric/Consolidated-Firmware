import 'dart:async';

import 'package:flutter/services.dart';

/// The steering wheel's buttons, including the rotaries' push switches.
enum DashboardButton {
  left,
  right,
  ok,
  telem,
  torqueVectoringPush,
  regenPush,
  driveModePush,
}

/// The steering wheel's rotary encoders.
enum DashboardRotary { torqueVectoring, regen, driveMode }

/// One driver input from the steering wheel.
sealed class DashboardInput {
  const DashboardInput();
}

class ButtonPressed extends DashboardInput {
  const ButtonPressed(this.button);

  final DashboardButton button;

  @override
  String toString() => 'ButtonPressed(${button.name})';
}

/// Sent when a button is let go; only needed for press-and-hold.
class ButtonReleased extends DashboardInput {
  const ButtonReleased(this.button);

  final DashboardButton button;

  @override
  String toString() => 'ButtonReleased(${button.name})';
}

/// [rotary] turned by [steps]: positive is clockwise, negative counter-clockwise.
class RotaryTurned extends DashboardInput {
  const RotaryTurned(this.rotary, this.steps);

  final DashboardRotary rotary;
  final int steps;

  @override
  String toString() => 'RotaryTurned(${rotary.name}, $steps)';
}

/// Driver input (buttons, rotary encoders) from the steering wheel.
///
/// PLACEHOLDER: the wheel's physical inputs aren't decided yet. The plan is for
/// Linux to expose them as key events (device-tree `gpio-keys` for buttons,
/// `rotary-encoder` for knobs), so they arrive here like keyboard keys. Until
/// then a keyboard stands in:
///
/// | Keys | Input |
/// |---|---|
/// | A / D | [DashboardButton.left] / [DashboardButton.right] |
/// | Return | [DashboardButton.ok] |
/// | T or Esc | [DashboardButton.telem] |
/// | 1 / 2 | Torque vectoring rotary, counter-clockwise / clockwise |
/// | 3 / 4 | Regen rotary, counter-clockwise / clockwise |
/// | Left / Right arrow | Drive mode rotary, counter-clockwise / clockwise |
/// | Space | Drive mode rotary push |
/// | Q / E (hold) | Torque vectoring / regen rotary push |
///
/// Holding a rotary key keeps turning it, like spinning a knob. Replace these
/// with the keycodes the wheel's device tree emits. Development-only keys live
/// in debug_keys.dart.
class InputService {
  static final Map<LogicalKeyboardKey, DashboardButton> _buttonKeys = {
    LogicalKeyboardKey.keyA: DashboardButton.left,
    LogicalKeyboardKey.keyD: DashboardButton.right,
    LogicalKeyboardKey.enter: DashboardButton.ok,
    LogicalKeyboardKey.keyT: DashboardButton.telem,
    LogicalKeyboardKey.escape: DashboardButton.telem,
    LogicalKeyboardKey.space: DashboardButton.driveModePush,
    LogicalKeyboardKey.keyQ: DashboardButton.torqueVectoringPush,
    LogicalKeyboardKey.keyE: DashboardButton.regenPush,
  };

  static final Map<LogicalKeyboardKey, (DashboardRotary, int)> _rotaryKeys = {
    LogicalKeyboardKey.digit1: (DashboardRotary.torqueVectoring, -1),
    LogicalKeyboardKey.digit2: (DashboardRotary.torqueVectoring, 1),
    LogicalKeyboardKey.digit3: (DashboardRotary.regen, -1),
    LogicalKeyboardKey.digit4: (DashboardRotary.regen, 1),
    LogicalKeyboardKey.arrowLeft: (DashboardRotary.driveMode, -1),
    LogicalKeyboardKey.arrowRight: (DashboardRotary.driveMode, 1),
  };

  /// The keys that stand in for [button], for showing next to debug controls.
  static List<String> keyLabelsFor(DashboardButton button) => [
        for (final MapEntry(:key, :value) in _buttonKeys.entries)
          if (value == button) key.keyLabel,
      ];

  static List<String> keyLabelsForRotary(DashboardRotary rotary, int steps) => [
        for (final MapEntry(:key, :value) in _rotaryKeys.entries)
          if (value == (rotary, steps)) key.keyLabel,
      ];

  final StreamController<DashboardInput> _inputs = StreamController.broadcast();

  /// Driver inputs, in the order they happen.
  Stream<DashboardInput> get inputs => _inputs.stream;

  void start() => HardwareKeyboard.instance.addHandler(_onKey);

  /// Injects an input as if it came from the wheel (debug panel buttons).
  void emit(DashboardInput input) => _inputs.add(input);

  bool _onKey(KeyEvent event) {
    final button = _buttonKeys[event.logicalKey];
    if (button != null) {
      if (event is KeyDownEvent) _inputs.add(ButtonPressed(button));
      if (event is KeyUpEvent) _inputs.add(ButtonReleased(button));
      return true;
    }

    final rotary = _rotaryKeys[event.logicalKey];
    if (rotary != null) {
      final (which, steps) = rotary;
      if (event is! KeyUpEvent) _inputs.add(RotaryTurned(which, steps));
      return true;
    }

    return false;
  }

  void dispose() {
    HardwareKeyboard.instance.removeHandler(_onKey);
    _inputs.close();
  }
}
