import 'dart:ffi' as ffi;
import 'dart:io';
import 'dart:isolate';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';

import 'package:dimos/data/services/can_frames.dart';
import 'package:dimos/data/services/jsoncan_ffi.dart';

typedef CanBatchCallback = void Function(Uint8List batch);
typedef CanErrorCallback = void Function(String error);

/// Where CAN frames come from. Both sources deliver batches in the
/// `can_frames.dart` layout, so the rest of the app doesn't care which is used.
abstract class CanSource {
  Future<void> start(CanBatchCallback onBatch, CanErrorCallback onError);
}

/// Reads classic and CAN FD frames from a SocketCAN interface (Linux only) using
/// the native reader in libdimos_jsoncan, on a separate isolate since reads block.
class SocketCanSource implements CanSource {
  SocketCanSource(this.interfaceName);

  final String interfaceName;

  static const int _maxFramesPerBatch = 256;
  static const int _maxBatchAgeMs = 10;

  @override
  Future<void> start(CanBatchCallback onBatch, CanErrorCallback onError) async {
    if (!Platform.isLinux) {
      onError('SocketCAN is Linux only; use DIMOS_CAN_SOURCE=udp with tool/can_sim');
      return;
    }
    final receivePort = ReceivePort();
    receivePort.listen((message) {
      if (message is Uint8List) {
        onBatch(message);
      } else if (message is String) {
        onError(message);
      }
    });
    await Isolate.spawn(_worker, (receivePort.sendPort, interfaceName));
  }

  static void _worker((SendPort, String) args) {
    final (mainSendPort, interfaceName) = args;

    final _NativeCanSocket socket;
    try {
      socket = _NativeCanSocket(openDimosNativeLibrary());
    } catch (e) {
      mainSendPort.send('CAN reader unavailable: $e');
      return;
    }

    final fd = socket.open(interfaceName);
    if (fd < 0) {
      mainSendPort.send('$interfaceName: ${_describeErrno(-fd)}');
      return;
    }

    final canId = calloc<ffi.Uint32>();
    final length = calloc<ffi.Uint8>();
    final data = calloc<ffi.Uint8>(JsonCanFfi.maxPayloadBytes);
    final batch = CanFrameBatchBuilder();
    final batchAge = Stopwatch();

    while (true) {
      // Block for the first frame of a batch, then drain what's already queued.
      final result = socket.read(fd, batch.isEmpty ? 100 : 0, canId, length, data);
      if (result < 0) {
        mainSendPort.send('$interfaceName: ${_describeErrno(-result)}');
        break;
      }
      if (result == 1) {
        if (batch.isEmpty) batchAge.reset();
        batchAge.start();
        batch.add(canId.value, data.asTypedList(length.value));
      }
      if (!batch.isEmpty &&
          (result == 0 ||
              batch.count >= _maxFramesPerBatch ||
              batchAge.elapsedMilliseconds >= _maxBatchAgeMs)) {
        mainSendPort.send(batch.take());
      }
    }

    socket.close(fd);
    calloc.free(canId);
    calloc.free(length);
    calloc.free(data);
  }

  // Linux errno values returned by native/can_socket.
  static String _describeErrno(int errno) => switch (errno) {
        1 || 13 => 'permission denied',
        19 => 'interface not found',
        97 => 'kernel has no SocketCAN support (sudo modprobe can_raw)',
        100 => 'interface is down (sudo ip link set <iface> up)',
        _ => 'socket error (errno $errno)',
      };
}

/// Receives frame batches over UDP on localhost, sent by tool/can_sim. Used for
/// development on machines without SocketCAN (e.g. macOS).
class UdpCanSource implements CanSource {
  UdpCanSource(this.port);

  final int port;

  @override
  Future<void> start(CanBatchCallback onBatch, CanErrorCallback onError) async {
    final RawDatagramSocket socket;
    try {
      socket = await RawDatagramSocket.bind(InternetAddress.loopbackIPv4, port);
    } catch (e) {
      onError('UDP port $port: $e');
      return;
    }
    socket.listen((event) {
      if (event != RawSocketEvent.read) return;
      final datagram = socket.receive();
      if (datagram != null) onBatch(datagram.data);
    });
  }
}

class _NativeCanSocket {
  _NativeCanSocket(ffi.DynamicLibrary lib)
      : _open = lib.lookupFunction<ffi.Int32 Function(ffi.Pointer<Utf8>),
            int Function(ffi.Pointer<Utf8>)>('dimos_can_open'),
        read = lib.lookupFunction<
            ffi.Int32 Function(ffi.Int32, ffi.Int32, ffi.Pointer<ffi.Uint32>,
                ffi.Pointer<ffi.Uint8>, ffi.Pointer<ffi.Uint8>),
            int Function(int, int, ffi.Pointer<ffi.Uint32>,
                ffi.Pointer<ffi.Uint8>, ffi.Pointer<ffi.Uint8>)>('dimos_can_read'),
        close = lib.lookupFunction<ffi.Void Function(ffi.Int32),
            void Function(int)>('dimos_can_close');

  final int Function(ffi.Pointer<Utf8>) _open;
  final int Function(int fd, int timeoutMs, ffi.Pointer<ffi.Uint32> canId,
      ffi.Pointer<ffi.Uint8> length, ffi.Pointer<ffi.Uint8> data) read;
  final void Function(int fd) close;

  int open(String interfaceName) {
    final name = interfaceName.toNativeUtf8(allocator: calloc);
    try {
      return _open(name);
    } finally {
      calloc.free(name);
    }
  }
}
