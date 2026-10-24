import 'dart:async';
import 'dart:math';

import 'package:flutter/foundation.dart';

import 'package:dimos/data/input/input_service.dart';
import 'package:dimos/routing/input_router.dart';

typedef Cell = (int, int);

/// Snake in the speed box, for when the car is parked in INIT. While running it
/// takes LEFT/RIGHT (turn) and OK (retry), ahead of everything else in the
/// input chain. Started with LEFT + RIGHT + OK pressed together, see [SnakeChord].
class SnakeGame extends ChangeNotifier implements InputMiddleware {
  static const int columns = 25;
  static const int rows = 13;
  static const Duration tick = Duration(milliseconds: 140);

  // Right, down, left, up: +1 turns clockwise.
  static const List<Cell> _directions = [(1, 0), (0, 1), (-1, 0), (0, -1)];

  final Random _random = Random();

  bool _running = false;
  List<Cell> _body = [];
  Cell _food = (0, 0);
  int _direction = 0;
  int _nextDirection = 0;
  bool _waiting = true;
  bool _dead = false;
  Timer? _timer;

  bool get running => _running;
  List<Cell> get body => _body;
  Cell get food => _food;
  bool get dead => _dead;
  bool get waiting => _waiting;
  int get score => _body.length - 3;

  String get status => _waiting && !_dead
      ? 'L/R TO START'
      : _dead
          ? 'SCORE $score · OK RETRY'
          : 'SCORE $score';

  void start() {
    _running = true;
    _reset();
  }

  void stop() {
    if (!_running) return;
    _timer?.cancel();
    _running = false;
    notifyListeners();
  }

  @override
  bool handleInput(DashboardInput input) {
    if (!_running) return false;
    switch (input) {
      case ButtonPressed(button: DashboardButton.left):
        _turn(-1);
      case ButtonPressed(button: DashboardButton.right):
        _turn(1);
      case ButtonPressed(button: DashboardButton.ok):
        if (_dead) _reset();
      default:
        return false;
    }
    return true;
  }

  void _reset() {
    const y = rows ~/ 2;
    _body = [(5, y), (4, y), (3, y)];
    _direction = _nextDirection = 0;
    _waiting = true;
    _dead = false;
    _placeFood();
    _timer?.cancel();
    _timer = Timer.periodic(tick, (_) => _step());
    notifyListeners();
  }

  void _turn(int by) {
    if (_dead) return;
    if (_waiting) {
      _waiting = false;
      notifyListeners();
      return;
    }
    _nextDirection = (_direction + by) % _directions.length;
  }

  void _placeFood() {
    do {
      _food = (_random.nextInt(columns), _random.nextInt(rows));
    } while (_body.contains(_food));
  }

  void _step() {
    if (_dead || _waiting) return;
    _direction = _nextDirection;
    final (dx, dy) = _directions[_direction];
    final (x, y) = _body.first;
    final head = (x + dx, y + dy);
    final (hx, hy) = head;
    if (hx < 0 || hy < 0 || hx >= columns || hy >= rows || _body.contains(head)) {
      _dead = true;
      _timer?.cancel();
      notifyListeners();
      return;
    }
    _body = [head, ..._body];
    if (head == _food) {
      _placeFood();
    } else {
      _body.removeLast();
    }
    notifyListeners();
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }
}

/// First in the input chain: LEFT, RIGHT and OK all pressed within [window]
/// calls [onChord]. Never consumes inputs itself, except the press completing
/// the chord.
class SnakeChord implements InputMiddleware {
  SnakeChord(this.onChord);

  static const Duration window = Duration(milliseconds: 600);
  static const Set<DashboardButton> _chord = {
    DashboardButton.left,
    DashboardButton.right,
    DashboardButton.ok,
  };

  final void Function() onChord;
  final Map<DashboardButton, DateTime> _pressedAt = {};

  @override
  bool handleInput(DashboardInput input) {
    if (input is! ButtonPressed || !_chord.contains(input.button)) return false;
    final now = DateTime.now();
    _pressedAt[input.button] = now;
    final complete = _chord.every((b) {
      final at = _pressedAt[b];
      return at != null && now.difference(at) < window;
    });
    if (!complete) return false;
    _pressedAt.clear();
    onChord();
    return true;
  }
}
