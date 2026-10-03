import 'dart:ffi' as ffi;
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';

// Opens libdimos_jsoncan (native/). On Linux it sits in the bundle's lib/ (on the
// runner's rpath); on macOS the Xcode build copies it into the app's Frameworks/.
ffi.DynamicLibrary openDimosNativeLibrary({String? libraryPath}) {
  if (libraryPath != null) return ffi.DynamicLibrary.open(libraryPath);
  if (Platform.isLinux) return ffi.DynamicLibrary.open('libdimos_jsoncan.so');
  if (Platform.isMacOS) {
    final contents = File(Platform.resolvedExecutable).parent.parent.path;
    return ffi.DynamicLibrary.open('$contents/Frameworks/libdimos_jsoncan.dylib');
  }
  throw UnsupportedError('libdimos_jsoncan is only built for Linux and macOS.');
}

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

  // Reused for every frame; only touched from the main isolate.
  final ffi.Pointer<ffi.Uint8> _frameBuffer = calloc<ffi.Uint8>(maxPayloadBytes);

  void ensureLoaded({String? libraryPath}) {
    if (_initialized) return;

    _lib = openDimosNativeLibrary(libraryPath: libraryPath);

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
    _frameBuffer.asTypedList(dlc).setRange(0, dlc, data);
    _processFrame(stdId, dlc, _frameBuffer);
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
