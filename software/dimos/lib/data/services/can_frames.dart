import 'dart:typed_data';

/// CAN frames move from a source to the app in batches packed into one buffer:
/// repeated `[u32 little-endian CAN id][u8 payload length][payload bytes]`.
/// tool/can_sim sends UDP datagrams in this same layout.
const int canFrameHeaderBytes = 5;

void forEachCanFrame(
  Uint8List batch,
  void Function(int canId, Uint8List data) onFrame,
) {
  final view = ByteData.sublistView(batch);
  var offset = 0;
  while (offset + canFrameHeaderBytes <= batch.length) {
    final canId = view.getUint32(offset, Endian.little);
    final length = batch[offset + 4];
    final start = offset + canFrameHeaderBytes;
    if (start + length > batch.length) return; // Truncated batch.
    onFrame(canId, Uint8List.sublistView(batch, start, start + length));
    offset = start + length;
  }
}

class CanFrameBatchBuilder {
  final BytesBuilder _bytes = BytesBuilder();
  final ByteData _header = ByteData(canFrameHeaderBytes);
  int _count = 0;

  int get count => _count;
  bool get isEmpty => _count == 0;

  void add(int canId, Uint8List data) {
    _header.setUint32(0, canId, Endian.little);
    _header.setUint8(4, data.length);
    _bytes.add(_header.buffer.asUint8List());
    _bytes.add(data);
    _count++;
  }

  Uint8List take() {
    _count = 0;
    return _bytes.takeBytes();
  }
}
