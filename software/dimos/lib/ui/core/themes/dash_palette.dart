import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/services/can_signals.dart';

/// The dashboard's colours, night (white on black) or day (black on white).
class DashPalette {
  const DashPalette._({
    required this.isNight,
    required this.background,
    required this.foreground,
    required this.off,
    required this.cell,
    required this.torqueVectoring,
    required this.regenStrength,
    required this.mode,
    required this.pink,
    required this.green,
    required this.yellow,
    required this.red,
    required this.regen,
    required this.throttle,
    required this.brake,
    required this.bad,
    required this.ok,
    required this.disabled,
    required this.downNode,
    required this.downNodeText,
    required this.states,
  });

  final bool isNight;
  final Color background;
  final Color foreground;

  /// Unlit segments and empty bars.
  final Color off;

  /// Tiles on the faults page.
  final Color cell;
  final Color torqueVectoring;
  final Color regenStrength;
  final Color mode;
  final Color pink;
  final Color green;
  final Color yellow;
  final Color red;
  final Color regen;
  final Color throttle;
  final Color brake;
  final Color bad;
  final Color ok;

  /// TV/RS tiles when switched off or locked.
  final Color disabled;

  /// Shutdown nodes after the first open one.
  final Color downNode;
  final Color downNodeText;

  /// State bar (background, text) for each [SystemState].
  final Map<SystemState, (Color, Color)> states;

  static const night = DashPalette._(
    isNight: true,
    background: Color(0xFF000000),
    foreground: Color(0xFFFFFFFF),
    off: Color(0xFF262626),
    cell: Color(0xFF1A1A1A),
    torqueVectoring: Color(0xFF0057FF),
    regenStrength: Color(0xFF00913D),
    mode: Color(0xFFC4C4C4),
    pink: Color(0xFFFF6EC0),
    green: Color(0xFF00E05A),
    yellow: Color(0xFFFFD400),
    red: Color(0xFFFF2020),
    regen: Color(0xFF5CC8FF),
    throttle: Color(0xFFFFFFFF),
    brake: Color(0xFFFF2020),
    bad: Color(0xFFE00000),
    ok: Color(0xFF00E05A),
    disabled: Color(0xFF4A4A4A),
    downNode: Color(0xFF3A3A3A),
    downNodeText: Color(0xFF9A9A9A),
    states: {
      SystemState.init: (Color(0xFFFFFFFF), Color(0xFF000000)),
      SystemState.readyToDrive: (Color(0xFFFFD400), Color(0xFF000000)),
      SystemState.drive: (Color(0xFF00E05A), Color(0xFF000000)),
      SystemState.fault: (Color(0xFFE00000), Color(0xFFFFFFFF)),
    },
  );

  static const day = DashPalette._(
    isNight: false,
    background: Color(0xFFFFFFFF),
    foreground: Color(0xFF000000),
    off: Color(0xFFDCDCDC),
    cell: Color(0xFFE6E6E6),
    torqueVectoring: Color(0xFF0047D6),
    regenStrength: Color(0xFF007A33),
    mode: Color(0xFFE2E2E2),
    pink: Color(0xFFFF5CB0),
    green: Color(0xFF00913D),
    yellow: Color(0xFFE6A800),
    red: Color(0xFFD00000),
    regen: Color(0xFF0A8FD6),
    throttle: Color(0xFF000000),
    brake: Color(0xFFD00000),
    bad: Color(0xFFCC0000),
    ok: Color(0xFF007A33),
    disabled: Color(0xFF6B6B6B),
    downNode: Color(0xFFA8A8A8),
    downNodeText: Color(0xFF333333),
    states: {
      SystemState.init: (Color(0xFF000000), Color(0xFFFFFFFF)),
      SystemState.readyToDrive: (Color(0xFFFFCC00), Color(0xFF000000)),
      SystemState.drive: (Color(0xFF007A33), Color(0xFFFFFFFF)),
      SystemState.fault: (Color(0xFFCC0000), Color(0xFFFFFFFF)),
    },
  );

  static DashPalette of(BuildContext context) =>
      context.select<DisplaySettings, bool>((s) => s.night) ? night : day;
}

/// The dashboard's heavy, tabular-figure type. Sizes are in the 1280 × 480
/// design's pixels; the whole frame is scaled to the screen.
TextStyle dashText(double size, {FontWeight weight = FontWeight.w900, Color? color}) =>
    TextStyle(
      fontSize: size,
      fontWeight: weight,
      color: color,
      height: 1,
      fontFeatures: const [FontFeature.tabularFigures()],
    );
