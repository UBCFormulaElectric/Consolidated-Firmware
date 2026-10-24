import 'dart:ffi' as ffi;
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';

import 'package:dimos/data/services/can_signals.dart';

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

// Sizes from native/jsoncan_bridge/jsoncan_bridge.h.
const int _boardCount = 6;
const int _shutdownNodeCount = 16;
const int _shutdownLoopOk = 16;
const int _boardNeverSeen = 0xFFFFFFFF;

// Mirrors `DimosSignals` in jsoncan_bridge.h; change both together.
final class _DimosSignals extends ffi.Struct {
  @ffi.Float()
  external double vehicleVelocity;
  @ffi.Float()
  external double tsPower;
  @ffi.Float()
  external double pappsPercent;
  @ffi.Array(_boardCount)
  external ffi.Array<ffi.Uint32> boardAgeMs;
  @ffi.Array(_boardCount)
  external ffi.Array<ffi.Uint16> boardVitalsId;
  @ffi.Uint16()
  external int frontBrakePressure;
  @ffi.Array(_shutdownNodeCount)
  external ffi.Array<ffi.Uint8> shutdownNodeOk;
  @ffi.Uint8()
  external int firstFaultNode;
  @ffi.Uint8()
  external int vcState;
  @ffi.Uint8()
  external int vcCanLoggingRemainingErrors;
  @ffi.Uint8()
  external int bmsState;
  @ffi.Uint8()
  external int bmsSoc;
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
  late final void Function(ffi.Pointer<_DimosSignals> out) _readSignals;

  // Reused for every frame / read; only touched from the main isolate.
  final ffi.Pointer<ffi.Uint8> _frameBuffer = calloc<ffi.Uint8>(maxPayloadBytes);
  final ffi.Pointer<_DimosSignals> _signals = calloc<_DimosSignals>();

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
    _readSignals = _lib.lookupFunction<
        ffi.Void Function(ffi.Pointer<_DimosSignals>),
        void Function(ffi.Pointer<_DimosSignals>)>(
      'dimos_jsoncan_read_signals',
    );

    final nativeSize = _lib
        .lookupFunction<ffi.Uint32 Function(), int Function()>(
            'dimos_jsoncan_signals_size')();
    if (nativeSize != ffi.sizeOf<_DimosSignals>()) {
      throw StateError('DimosSignals is $nativeSize bytes in libdimos_jsoncan but '
          '${ffi.sizeOf<_DimosSignals>()} in Dart; update jsoncan_ffi.dart');
    }

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

  /// The current RX table values, in one native call.
  CanSnapshot readSignals() {
    ensureLoaded();
    _readSignals(_signals);
    final s = _signals.ref;
    return CanSnapshot(
      speedKph: s.vehicleVelocity,
      tsPowerKw: s.tsPower,
      throttlePercent: s.pappsPercent,
      frontBrakePsi: s.frontBrakePressure,
      socPercent: s.bmsSoc,
      vcState: s.vcState,
      bmsState: s.bmsState,
      canLoggingRemainingErrors: s.vcCanLoggingRemainingErrors,
      firstFaultNode: s.firstFaultNode >= _shutdownLoopOk ? null : s.firstFaultNode,
      shutdownNodeOk: [for (var i = 0; i < _shutdownNodeCount; i++) s.shutdownNodeOk[i] != 0],
      boardAgeMs: [
        for (var i = 0; i < _boardCount; i++)
          s.boardAgeMs[i] == _boardNeverSeen ? null : s.boardAgeMs[i]
      ],
      boardVitalsId: [for (var i = 0; i < _boardCount; i++) s.boardVitalsId[i]],
    );
  }
}
