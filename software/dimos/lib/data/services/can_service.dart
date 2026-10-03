import 'dart:async';
import 'dart:io';

import 'package:flutter/foundation.dart';

import 'package:dimos/data/services/can_api.dart';
import 'package:dimos/data/services/can_frames.dart';
import 'package:dimos/data/services/jsoncan_ffi.dart';

/// Where CAN frames come from, set with --dart-define (see README).
class CanConfig {
  const CanConfig({
    required this.source,
    required this.interfaceName,
    required this.udpPort,
  });

  /// `socketcan` (default on Linux) or `udp` (default elsewhere).
  final String source;
  final String interfaceName;
  final int udpPort;

  factory CanConfig.fromEnvironment() {
    const source = String.fromEnvironment('DIMOS_CAN_SOURCE');
    return CanConfig(
      source: source.isNotEmpty ? source : (Platform.isLinux ? 'socketcan' : 'udp'),
      interfaceName:
          const String.fromEnvironment('DIMOS_CAN_INTERFACE', defaultValue: 'can0'),
      udpPort: const int.fromEnvironment('DIMOS_CAN_UDP_PORT', defaultValue: 5005),
    );
  }
}

/// The whole CAN pipeline, independent of the UI: a [CanSource] feeds frames into
/// the jsoncan RX table as they arrive, and listeners are notified at most every
/// [refreshPeriod] when new frames have been decoded. Read values through [jsoncan].
class CanService extends ChangeNotifier {
  CanService({
    CanConfig? config,
    this.refreshPeriod = const Duration(milliseconds: 33),
  }) : config = config ?? CanConfig.fromEnvironment();

  final CanConfig config;
  final Duration refreshPeriod;
  final JsonCanFfi jsoncan = JsonCanFfi.instance;

  /// Errors from loading the decoder or from the CAN source, newest last.
  final ValueNotifier<List<String>> errors = ValueNotifier(const []);

  Timer? _refreshTimer;
  bool _framesSinceRefresh = false;

  void start() {
    try {
      jsoncan.ensureLoaded();
    } catch (e) {
      _reportError('jsoncan library not loaded: $e');
      return;
    }

    final CanSource source = config.source == 'udp'
        ? UdpCanSource(config.udpPort)
        : SocketCanSource(config.interfaceName);
    source.start(
      (batch) {
        forEachCanFrame(batch, jsoncan.processFrame);
        _framesSinceRefresh = true;
      },
      _reportError,
    );

    _refreshTimer = Timer.periodic(refreshPeriod, (_) {
      if (!_framesSinceRefresh) return;
      _framesSinceRefresh = false;
      notifyListeners();
    });
  }

  void _reportError(String error) {
    debugPrint('CAN: $error');
    errors.value = [...errors.value, error];
  }

  @override
  void dispose() {
    _refreshTimer?.cancel();
    errors.dispose();
    super.dispose();
  }
}
