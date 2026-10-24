import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/snake_game.dart';
import 'package:dimos/ui/core/themes/dash_palette.dart';

/// [SnakeGame], drawn over the speed box.
class SnakeView extends StatelessWidget {
  const SnakeView({super.key});

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final game = context.watch<SnakeGame>();
    return ColoredBox(
      color: palette.background,
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Container(
            height: 28,
            color: palette.foreground,
            padding: const EdgeInsets.symmetric(horizontal: 10),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text('SNAKE', style: dashText(14, color: palette.background)),
                Text(game.status, style: dashText(14, color: palette.background)),
              ],
            ),
          ),
          Expanded(
            child: CustomPaint(painter: _SnakePainter(game, palette), size: Size.infinite),
          ),
        ],
      ),
    );
  }
}

class _SnakePainter extends CustomPainter {
  _SnakePainter(this.game, this.palette)
      : body = List.of(game.body),
        food = game.food,
        dead = game.dead;

  final SnakeGame game;
  final DashPalette palette;
  // Copied so shouldRepaint can tell when the game moved.
  final List<Cell> body;
  final Cell food;
  final bool dead;

  @override
  void paint(Canvas canvas, Size size) {
    final cell = (size.width / SnakeGame.columns)
        .clamp(0.0, size.height / SnakeGame.rows)
        .floorToDouble();
    final origin = Offset(
      ((size.width - SnakeGame.columns * cell) / 2).floorToDouble(),
      ((size.height - SnakeGame.rows * cell) / 2).floorToDouble(),
    );
    final board = origin & Size(SnakeGame.columns * cell, SnakeGame.rows * cell);
    canvas.drawRect(
      board.deflate(1),
      Paint()
        ..color = palette.off
        ..style = PaintingStyle.stroke
        ..strokeWidth = 2,
    );

    Rect square((int, int) c, double inset) =>
        Rect.fromLTWH(origin.dx + c.$1 * cell, origin.dy + c.$2 * cell, cell, cell)
            .deflate(inset);

    canvas.drawRect(square(food, 3), Paint()..color = palette.red);
    for (final (i, c) in body.indexed) {
      final color = dead
          ? palette.off
          : i == 0
              ? palette.foreground
              : palette.green;
      canvas.drawRect(square(c, 1), Paint()..color = color);
    }
  }

  @override
  bool shouldRepaint(_SnakePainter old) =>
      old.food != food ||
      old.dead != dead ||
      old.palette != palette ||
      old.body.length != body.length ||
      (body.isNotEmpty && old.body.first != body.first);
}
