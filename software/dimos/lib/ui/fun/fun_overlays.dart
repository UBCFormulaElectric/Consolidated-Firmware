import 'package:flutter/material.dart';

import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/services/can_signals.dart';

/// The "Challenge Complete!" toast that slides in from the right, for
/// FunEffects.achievement. Drawn rather than an image so it needs no asset.
class AchievementToast extends StatefulWidget {
  const AchievementToast({super.key});

  @override
  State<AchievementToast> createState() => _AchievementToastState();
}

class _AchievementToastState extends State<AchievementToast>
    with SingleTickerProviderStateMixin {
  late final AnimationController _controller =
      AnimationController(vsync: this, duration: FunEffects.achievementDuration)..forward();

  // Off-screen right → in by 8% → hold → out from 88%.
  late final Animation<double> _x = TweenSequence([
    TweenSequenceItem(tween: Tween(begin: 1.1, end: 0.0), weight: 8),
    TweenSequenceItem(tween: ConstantTween(0.0), weight: 80),
    TweenSequenceItem(tween: Tween(begin: 0.0, end: 1.1), weight: 12),
  ]).chain(CurveTween(curve: Curves.easeInOut)).animate(_controller);

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    const mono = TextStyle(
      fontFamily: 'monospace',
      fontSize: 22,
      height: 1.15,
      fontWeight: FontWeight.w700,
    );
    final title = mono.copyWith(color: const Color(0xFFD27BE6));
    final subtitle = mono.copyWith(color: Colors.white);
    return Positioned(
      top: 10,
      right: 10,
      width: 368,
      height: 77,
      child: IgnorePointer(
        child: AnimatedBuilder(
          animation: _x,
          builder: (context, child) =>
              FractionalTranslation(translation: Offset(_x.value, 0), child: child),
          child: Container(
            decoration: BoxDecoration(
              color: const Color(0xFF212121),
              border: Border.all(color: const Color(0xFF555555), width: 4),
              borderRadius: BorderRadius.circular(6),
            ),
            padding: const EdgeInsets.symmetric(horizontal: 14),
            child: Row(
              spacing: 12,
              children: [
                const Icon(Icons.local_drink, color: Color(0xFFDDDDDD), size: 34),
                Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text('Challenge Complete!', style: title),
                    Text('How Did We Get Here?', style: subtitle),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

/// A camera-style "screenshot" of a fault: white flash, then the picture
/// shrinks into a thumbnail bottom-left and slides away, for FunEffects.shotNode.
///
/// Shows `assets/fun/screenshot-<node>.png` if the team adds one (imd, bspd,
/// bms), otherwise a placeholder card with the node's name.
class FaultShot extends StatefulWidget {
  const FaultShot({super.key, required this.node});

  final int node;

  @override
  State<FaultShot> createState() => _FaultShotState();
}

class _FaultShotState extends State<FaultShot> with SingleTickerProviderStateMixin {
  static const Duration _duration = Duration(seconds: 4);
  static const double _flashFraction = 0.15; // 0.6 s of the 4 s

  static const Map<int, (String, Color)> _shots = {
    ShutdownNodes.imd: ('imd', Color(0xFF5A2424)),
    ShutdownNodes.bspd: ('bspd', Color(0xFF243A5A)),
    ShutdownNodes.bmsOk: ('bms', Color(0xFF2E4A2A)),
  };

  late final AnimationController _controller =
      AnimationController(vsync: this, duration: _duration)..forward();

  late final Animation<double> _shrink = TweenSequence([
    TweenSequenceItem(tween: ConstantTween(0.0), weight: 12),
    TweenSequenceItem(tween: Tween(begin: 0.0, end: 1.0), weight: 18),
    TweenSequenceItem(tween: ConstantTween(1.0), weight: 70),
  ]).chain(CurveTween(curve: Curves.easeInOut)).animate(_controller);

  late final Animation<double> _slide = TweenSequence([
    TweenSequenceItem(tween: ConstantTween(0.0), weight: 82),
    TweenSequenceItem(tween: Tween(begin: 0.0, end: 1.0), weight: 18),
  ]).chain(CurveTween(curve: Curves.easeInOut)).animate(_controller);

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final (file, color) = _shots[widget.node]!;
    final name = shutdownLoop[widget.node].name;
    final picture = Image.asset(
      'assets/fun/screenshot-$file.png',
      fit: BoxFit.cover,
      errorBuilder: (context, error, stack) => ColoredBox(
        color: color,
        child: Center(
          child: Text(name,
              style: const TextStyle(
                  color: Colors.white, fontSize: 96, fontWeight: FontWeight.w900)),
        ),
      ),
    );

    return IgnorePointer(
      child: AnimatedBuilder(
        animation: _controller,
        builder: (context, _) {
          final t = _shrink.value;
          final scale = 1 - 0.76 * t;
          final flash = (1 - _controller.value / _flashFraction).clamp(0.0, 1.0);
          return Stack(
            fit: StackFit.expand,
            children: [
              Transform(
                alignment: Alignment.bottomLeft,
                transform: Matrix4.identity()
                  ..translateByDouble(16 * t - 436 * _slide.value, -16 * t, 0, 1)
                  ..scaleByDouble(scale, scale, 1, 1),
                child: Container(
                  foregroundDecoration: BoxDecoration(
                    border: Border.all(color: Colors.white, width: 18 * t),
                    borderRadius: BorderRadius.circular(50 * t),
                  ),
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(50 * t),
                    boxShadow: const [BoxShadow(color: Color(0x80000000), blurRadius: 60)],
                  ),
                  clipBehavior: Clip.antiAlias,
                  child: picture,
                ),
              ),
              if (flash > 0) ColoredBox(color: Colors.white.withValues(alpha: flash)),
            ],
          );
        },
      ),
    );
  }
}
