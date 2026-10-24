import 'package:flutter/material.dart';

import 'package:dimos/data/input/debug_keys.dart';
import 'package:dimos/data/input/input_service.dart';

/// Development-only controls under the dashboard (toggle with `), one button
/// per wheel input and debug action, each labelled with its keyboard key.
class DebugPanel extends StatelessWidget {
  const DebugPanel({super.key, required this.onInput, required this.onAction});

  final void Function(DashboardInput input) onInput;
  final void Function(DebugAction action) onAction;

  @override
  Widget build(BuildContext context) {
    Widget press(String label, DashboardButton button, Color color) => _DebugButton(
          label: label,
          keys: InputService.keyLabelsFor(button),
          color: color,
          onDown: () => onInput(ButtonPressed(button)),
          onUp: () => onInput(ButtonReleased(button)),
        );
    Widget turn(String label, DashboardRotary rotary, int steps, Color color) => _DebugButton(
          label: label,
          keys: InputService.keyLabelsForRotary(rotary, steps),
          color: color,
          onDown: () => onInput(RotaryTurned(rotary, steps)),
        );

    const blue = Color(0xFF6F9CFF);
    const green = Color(0xFF3FD67A);
    const grey = Color(0xFFBDBDBD);
    const white = Color(0xFFEEEEEE);

    return Container(
      color: const Color(0xFF161616),
      padding: const EdgeInsets.all(10),
      child: Wrap(
        spacing: 10,
        runSpacing: 10,
        children: [
          _Group('WHEEL', [
            press('◀ LEFT', DashboardButton.left, white),
            press('RIGHT ▶', DashboardButton.right, white),
            press('TELEM', DashboardButton.telem, const Color(0xFFFF7AC2)),
            press('OK', DashboardButton.ok, white),
          ]),
          _Group('ROTARIES', [
            turn('TV −', DashboardRotary.torqueVectoring, -1, blue),
            turn('TV +', DashboardRotary.torqueVectoring, 1, blue),
            press('TV PUSH · hold 1s', DashboardButton.torqueVectoringPush, blue),
            turn('RS −', DashboardRotary.regen, -1, green),
            turn('RS +', DashboardRotary.regen, 1, green),
            press('RS PUSH · hold 1s', DashboardButton.regenPush, green),
            turn('MODE ◀', DashboardRotary.driveMode, -1, grey),
            turn('MODE ▶', DashboardRotary.driveMode, 1, grey),
            press('MODE PUSH', DashboardButton.driveModePush, grey),
          ]),
          _Group('DEBUG', [
            for (final action in DebugAction.values)
              _DebugButton(
                label: action.label,
                keys: [action.key.keyLabel],
                color: switch (action) {
                  DebugAction.systemStep => const Color(0xFFFFD400),
                  DebugAction.injectFault || DebugAction.pressEstop => const Color(0xFFFF6A6A),
                  DebugAction.faultImd ||
                  DebugAction.faultBspd ||
                  DebugAction.faultBmsOk =>
                    const Color(0xFFFFB36B),
                  DebugAction.snake => const Color(0xFF7DFF9A),
                  DebugAction.funFx => const Color(0xFFC58AFF),
                  _ => white,
                },
                onDown: () => onAction(action),
              ),
          ]),
        ],
      ),
    );
  }
}

class _Group extends StatelessWidget {
  const _Group(this.title, this.buttons);

  final String title;
  final List<Widget> buttons;

  @override
  Widget build(BuildContext context) => Container(
        padding: const EdgeInsets.symmetric(horizontal: 14, vertical: 12),
        decoration: BoxDecoration(
          color: const Color(0xFF0E0E0E),
          border: Border.all(color: const Color(0xFF2A2A2A)),
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisSize: MainAxisSize.min,
          spacing: 6,
          children: [
            Text(title,
                style: const TextStyle(
                    color: Color(0xFF8A8A8A), fontSize: 11, letterSpacing: 1.8)),
            Wrap(spacing: 8, runSpacing: 8, children: buttons),
          ],
        ),
      );
}

class _DebugButton extends StatelessWidget {
  const _DebugButton({
    required this.label,
    required this.keys,
    required this.color,
    required this.onDown,
    this.onUp,
  });

  final String label;
  final List<String> keys;
  final Color color;
  final VoidCallback onDown;
  final VoidCallback? onUp;

  @override
  Widget build(BuildContext context) => Listener(
        // Press/release, not tap, so hold-to-toggle works with the mouse too.
        onPointerDown: (_) => onDown(),
        onPointerUp: (_) => onUp?.call(),
        onPointerCancel: (_) => onUp?.call(),
        child: MouseRegion(
          cursor: SystemMouseCursors.click,
          child: Container(
            padding: const EdgeInsets.symmetric(horizontal: 14, vertical: 10),
            decoration: BoxDecoration(
              color: const Color(0xFF1C1C1C),
              border: Border.all(color: const Color(0xFF333333)),
            ),
            child: Text.rich(
              TextSpan(children: [
                TextSpan(text: label, style: TextStyle(color: color)),
                if (keys.isNotEmpty)
                  TextSpan(
                    text: '  ${keys.join(' / ')}',
                    style: const TextStyle(color: Color(0xFF6A6A6A)),
                  ),
              ]),
              style: const TextStyle(fontSize: 13, fontFamily: 'monospace'),
            ),
          ),
        ),
      );
}
