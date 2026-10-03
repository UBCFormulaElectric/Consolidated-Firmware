import 'dart:ffi' as ffi;
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';

// Dart bindings for libdimos_jsoncan (native/jsoncan_bridge), which wraps the
// Rust jsoncan generated code for the DIMOS node.
class JsonCanFfi {
  JsonCanFfi._();

  static final JsonCanFfi instance = JsonCanFfi._();

  // CAN FD payloads are at most 64 bytes.
  static const int maxPayloadBytes = 64;

  bool _initialized = false;
  late final ffi.DynamicLibrary _lib;

  late final void Function() _init;
  late final void Function(int stdId, int dlc, ffi.Pointer<ffi.Uint8> data)
      _processFrame;
  late final int Function(int stdId, int timeMs) _needsLog;
  late final int Function(int stdId, int timeMs) _needsTelem;
  late final int Function() _getVcCanLoggingRemainingErrors;

  static String get _defaultLibraryPath {
    if (Platform.isLinux) return 'libdimos_jsoncan.so';
    if (Platform.isMacOS) return 'libdimos_jsoncan.dylib';
    throw UnsupportedError('JsonCanFfi is only supported on Linux and macOS.');
  }

  void ensureLoaded({String? libraryPath}) {
    if (_initialized) return;

    _lib = ffi.DynamicLibrary.open(libraryPath ?? _defaultLibraryPath);

    _init = _lib.lookupFunction<ffi.Void Function(), void Function()>(
      'dimos_jsoncan_init',
    );
    _processFrame = _lib.lookupFunction<
        ffi.Void Function(ffi.Uint32, ffi.Uint32, ffi.Pointer<ffi.Uint8>),
        void Function(int, int, ffi.Pointer<ffi.Uint8>)>(
      'dimos_jsoncan_process_frame',
    );
    _needsLog = _lib.lookupFunction<ffi.Uint8 Function(ffi.Uint32, ffi.Uint32),
        int Function(int, int)>(
      'dimos_jsoncan_needs_log',
    );
    _needsTelem = _lib.lookupFunction<ffi.Uint8 Function(ffi.Uint32, ffi.Uint32),
        int Function(int, int)>(
      'dimos_jsoncan_needs_telem',
    );
    _getVcCanLoggingRemainingErrors =
        _lib.lookupFunction<ffi.Uint8 Function(), int Function()>(
      'dimos_jsoncan_get_vc_can_logging_remaining_errors',
    );

    _init();
    _initialized = true;
  }

  void processFrame(int stdId, Uint8List data) {
    ensureLoaded();

    final dlc = data.length > maxPayloadBytes ? maxPayloadBytes : data.length;
    final ptr = calloc<ffi.Uint8>(maxPayloadBytes);
    try {
      ptr.asTypedList(dlc).setRange(0, dlc, data);
      _processFrame(stdId, dlc, ptr);
    } finally {
      calloc.free(ptr);
    }
  }

  // Data capture for the DIMOS logger node; `timeMs` must be monotonic.
  bool needsLog(int stdId, int timeMs) {
    ensureLoaded();
    return _needsLog(stdId, timeMs) != 0;
  }

  bool needsTelem(int stdId, int timeMs) {
    ensureLoaded();
    return _needsTelem(stdId, timeMs) != 0;
  }

  int getVcCanLoggingRemainingErrors() {
    ensureLoaded();
    return _getVcCanLoggingRemainingErrors();
  }
}
