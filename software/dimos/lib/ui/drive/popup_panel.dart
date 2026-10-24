import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/driver_controls.dart';
import 'package:dimos/ui/core/shared_widgets/effects.dart';
import 'package:dimos/ui/core/themes/dash_palette.dart';

(Color, Color) _popupColors(DashPalette palette, PopupKind kind) => switch (kind) {
      PopupKind.torqueVectoring => (palette.torqueVectoring, Colors.white),
      PopupKind.regen => (palette.regenStrength, Colors.white),
      PopupKind.mode => (palette.mode, Colors.black),
    };

/// Right of the speed: the drive mode selector while it's open, otherwise the
/// latest setting popup, otherwise nothing.
class SidePanel extends StatelessWidget {
  const SidePanel({super.key});

  @override
  Widget build(BuildContext context) {
    final controls = context.watch<DriverControls>();
    final selection = controls.selection;
    if (selection != null) return _ModeSelector(controls, selection);

    final popup = controls.popup;
    if (popup == null) return const SizedBox.shrink();
    final (background, foreground) = _popupColors(DashPalette.of(context), popup.kind);
    final valueSize = popup.value.length > 4
        ? 38.0
        : popup.value.length > 3
            ? 54.0
            : 96.0;
    return Container(
      color: background,
      padding: const EdgeInsets.all(14),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 8,
        children: [
          Text(popup.title, textAlign: TextAlign.center, style: dashText(16, color: foreground)),
          Text(popup.value, style: dashText(valueSize, color: foreground)),
          Text(popup.hint,
              textAlign: TextAlign.center,
              style: dashText(13, weight: FontWeight.w800, color: foreground)),
        ],
      ),
    );
  }
}

class _ModeSelector extends StatelessWidget {
  const _ModeSelector(this.controls, this.selection);

  final DriverControls controls;
  final int selection;

  @override
  Widget build(BuildContext context) {
    final black = dashText(16, color: Colors.black);
    return Container(
      color: DashPalette.of(context).mode,
      padding: const EdgeInsets.all(10),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        spacing: 6,
        children: [
          Text('DRIVE MODE', textAlign: TextAlign.center, style: black),
          for (final (i, mode) in driveModes.indexed)
            Expanded(child: _modeRow(i, mode)),
          Text('PUSH TO CONFIRM',
              textAlign: TextAlign.center,
              style: dashText(13, weight: FontWeight.w800, color: Colors.black)),
        ],
      ),
    );
  }

  Widget _modeRow(int i, DriveMode mode) {
    final selected = i == selection;
    final locked = controls.isModeLocked(mode);
    final foreground = selected
        ? Colors.white
        : locked
            ? const Color(0xFF555555)
            : Colors.black;
    return Stack(
      fit: StackFit.expand,
      children: [
        ColoredBox(
          color: selected
              ? Colors.black
              : locked
                  ? const Color(0xFF9A9A9A)
                  : Colors.transparent,
        ),
        if (locked && !selected)
          const Stripes(color: Color(0x47000000), width: 6, period: 14),
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 12),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Text(mode.name, style: dashText(20, color: foreground)),
              if (i == controls.modeIndex)
                Text('ACTIVE', style: dashText(12, weight: FontWeight.w800, color: foreground)),
            ],
          ),
        ),
      ],
    );
  }
}

/// The setting popup in the middle of the screen, for pages other than drive.
class CenterPopup extends StatelessWidget {
  const CenterPopup({super.key});

  @override
  Widget build(BuildContext context) {
    final popup = context.select<DriverControls, Popup?>((c) => c.popup);
    if (popup == null) return const SizedBox.shrink();
    final palette = DashPalette.of(context);
    final (background, foreground) = _popupColors(palette, popup.kind);
    return Center(
      child: Container(
        constraints: const BoxConstraints(minWidth: 340),
        padding: const EdgeInsets.fromLTRB(36, 18, 36, 22),
        decoration: BoxDecoration(
          color: background,
          boxShadow: [BoxShadow(color: palette.background, spreadRadius: 6)],
        ),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text(popup.title, style: dashText(20, color: foreground)),
            Padding(
              padding: const EdgeInsets.only(top: 6, bottom: 8),
              child: Text(popup.value, style: dashText(112, color: foreground)),
            ),
            Text(popup.hint, style: dashText(16, weight: FontWeight.w800, color: foreground)),
          ],
        ),
      ),
    );
  }
}
