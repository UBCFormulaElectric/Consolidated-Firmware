import 'dart:math';

import 'package:flutter/material.dart';

/// Diagonal hazard stripes (135°): [width] of [color], then a gap, every [period].
class Stripes extends StatelessWidget {
  const Stripes({super.key, required this.color, this.width = 24, this.period = 48});

  final Color color;
  final double width;
  final double period;

  @override
  Widget build(BuildContext context) => IgnorePointer(
        child: CustomPaint(
          painter: _StripesPainter(color, width, period),
          size: Size.infinite,
        ),
      );
}

class _StripesPainter extends CustomPainter {
  _StripesPainter(this.color, this.width, this.period);

  final Color color;
  final double width;
  final double period;

  @override
  void paint(Canvas canvas, Size size) {
    canvas.clipRect(Offset.zero & size);
    final paint = Paint()..color = color;
    // Bands perpendicular to the 135° gradient direction, measured along x + y.
    final step = period * sqrt2;
    final band = width * sqrt2;
    for (var d = -size.height; d < size.width + size.height; d += step) {
      canvas.drawPath(
        Path()
          ..moveTo(d, 0)
          ..lineTo(d + band, 0)
          ..lineTo(d + band - size.height, size.height)
          ..lineTo(d - size.height, size.height)
          ..close(),
        paint,
      );
    }
  }

  @override
  bool shouldRepaint(_StripesPainter old) =>
      old.color != color || old.width != width || old.period != period;
}

/// Hard on/off blinking: [child] is at full opacity for the first half of each
/// [period] and [dimmed] for the second. [repeat] limits the number of blinks.
class Blink extends StatefulWidget {
  const Blink({
    super.key,
    required this.child,
    this.period = const Duration(milliseconds: 600),
    this.dimmed = 0.25,
    this.repeat,
    this.startDimmed = false,
  });

  final Widget child;
  final Duration period;
  final double dimmed;
  final int? repeat;
  final bool startDimmed;

  @override
  State<Blink> createState() => _BlinkState();
}

class _BlinkState extends State<Blink> with SingleTickerProviderStateMixin {
  late final AnimationController _controller =
      AnimationController(vsync: this, duration: widget.period);

  @override
  void initState() {
    super.initState();
    final repeat = widget.repeat;
    if (repeat == null) {
      _controller.repeat();
    } else {
      _controller.repeat(count: repeat);
    }
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) => AnimatedBuilder(
        animation: _controller,
        builder: (context, child) {
          final firstHalf = _controller.value < 0.5;
          final lit = firstHalf != widget.startDimmed && _controller.isAnimating;
          return Opacity(opacity: lit ? 1 : widget.dimmed, child: child);
        },
        child: widget.child,
      );
}

/// Steps through the rainbow, for when everything is broken at once.
class Rainbow extends StatefulWidget {
  const Rainbow({super.key, required this.seed, required this.child});

  /// Picks this tile's speed and phase, so neighbours don't flash in sync.
  final int seed;
  final Widget child;

  static const List<Color> colors = [
    Color(0xFFFF0040),
    Color(0xFFFF8A00),
    Color(0xFFFFE600),
    Color(0xFF00E05A),
    Color(0xFF00C8FF),
    Color(0xFF3A5CFF),
    Color(0xFFC040FF),
  ];

  @override
  State<Rainbow> createState() => _RainbowState();
}

class _RainbowState extends State<Rainbow> with SingleTickerProviderStateMixin {
  late final Random _random = Random(widget.seed);
  late final AnimationController _controller = AnimationController(
    vsync: this,
    duration: Duration(milliseconds: 500 + _random.nextInt(900)),
  )..repeat();
  late final double _phase = _random.nextDouble();

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) => AnimatedBuilder(
        animation: _controller,
        builder: (context, child) {
          final i = (((_controller.value + _phase) % 1) * Rainbow.colors.length).floor();
          return ColoredBox(color: Rainbow.colors[i], child: child);
        },
        child: widget.child,
      );
}
