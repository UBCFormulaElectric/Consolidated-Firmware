import 'dart:ffi' as ffi;
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';

class JsonCanFfi {
  JsonCanFfi._();

  static final JsonCanFfi instance = JsonCanFfi._();

  bool _initialized = false;
  late final ffi.DynamicLibrary _lib;

  late final void Function() _init;
  late final void Function(int stdId, int dlc, ffi.Pointer<ffi.Uint8> data)
      _processFrame;
  late final int Function() _getDemoU32;

  void ensureLoaded() {
    if (_initialized) return;
    if (!Platform.isLinux) {
      throw StateError('JsonCanFfi is only supported on Linux.');
    }

    _lib = ffi.DynamicLibrary.open('libdimos_jsoncan.so');

    _init = _lib.lookupFunction<ffi.Void Function(), void Function()>(
      'dimos_jsoncan_init',
    );
    _processFrame = _lib.lookupFunction<
        ffi.Void Function(ffi.Uint32, ffi.Uint32, ffi.Pointer<ffi.Uint8>),
        void Function(int, int, ffi.Pointer<ffi.Uint8>)>(
      'dimos_jsoncan_process_frame',
    );
    _getDemoU32 = _lib.lookupFunction<ffi.Uint32 Function(), int Function()>(
      'dimos_jsoncan_get_demo_u32',
    );

    _init();
    _initialized = true;
  }

  void processFrame(int stdId, Uint8List data) {
    ensureLoaded();

    // jsoncan expects at most 8 bytes.
    final dlc = data.length > 8 ? 8 : data.length;
    final ptr = calloc<ffi.Uint8>(8);
    try {
      for (var i = 0; i < dlc; i++) {
        ptr[i] = data[i];
      }
      _processFrame(stdId, dlc, ptr);
    } finally {
      calloc.free(ptr);
    }
  }

  int getDemoU32() {
    ensureLoaded();
    return _getDemoU32();
  }
}
