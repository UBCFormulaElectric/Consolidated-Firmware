import 'dart:async';
import 'dart:collection';
import 'dart:math';

import 'package:flutter/foundation.dart';

/// How an info message is drawn on the drive page.
enum InfoStyle {
  /// Yellow: something the driver should know about.
  warning,

  /// White with an "i": the driver's own change took effect.
  confirm,

  /// Pink: the driver marked a telemetry event.
  telem,

  /// Grey: the car changed a setting on its own.
  system,
}

class InfoMessage {
  const InfoMessage(this.source, this.message, this.style);

  final String source;
  final String message;
  final InfoStyle style;
}

/// The message column on the drive page: short-lived messages from driver
/// actions, plus warnings that stay until cleared (e.g. CAN errors).
class InfoFeed extends ChangeNotifier {
  static const Duration messageLifetime = Duration(seconds: 6);
  static const int maxShown = 5;

  final List<InfoMessage> _messages = [];
  final LinkedHashMap<String, InfoMessage> _warnings = LinkedHashMap();
  final List<Timer> _timers = [];

  /// Warnings first, then messages oldest to newest; at most [maxShown].
  List<InfoMessage> get shown {
    final all = [..._warnings.values, ..._messages];
    return all.sublist(max(0, all.length - maxShown));
  }

  void add(String source, String message, [InfoStyle style = InfoStyle.warning]) {
    final info = InfoMessage(source, message, style);
    _messages.add(info);
    if (_messages.length > maxShown) _messages.removeAt(0);
    late final Timer timer;
    timer = Timer(messageLifetime, () {
      _timers.remove(timer);
      if (_messages.remove(info)) notifyListeners();
    });
    _timers.add(timer);
    notifyListeners();
  }

  /// Shows or hides the warning [id]; showing it again replaces its text.
  void setWarning(String id, bool active, {String source = '', String? message}) {
    final changed = active
        ? _warnings[id]?.message != (message ?? id)
        : _warnings.containsKey(id);
    if (!changed) return;
    if (active) {
      _warnings[id] = InfoMessage(source, message ?? id, InfoStyle.warning);
    } else {
      _warnings.remove(id);
    }
    notifyListeners();
  }

  @override
  void dispose() {
    for (final timer in _timers) {
      timer.cancel();
    }
    super.dispose();
  }
}
