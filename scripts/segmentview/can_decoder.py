"""
CAN message decoding for BMS data
"""

import json
from pathlib import Path
from typing import Dict

import can
from PyQt5.QtCore import Qt, pyqtSignal, QObject

from .segments_model import SegmentData


class CanMessageDecoder:
    """Decodes CAN messages from BMS according to BMS_tx.json schema"""

    def __init__(self, config_path: Path):
        self.config = self._load_config(config_path)
        self.message_map = self._build_message_map()

    def _load_config(self, config_path: Path) -> Dict:
        """Load BMS configuration from JSON file"""
        try:
            with open(config_path, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Warning: Could not find BMS config at {config_path}")
            return {}

    def _build_message_map(self) -> Dict[int, Dict]:
        """Build a map of message ID to message definition"""
        msg_map = {}
        for msg_name, msg_def in self.config.items():
            if 'msg_id' in msg_def:
                msg_map[msg_def['msg_id']] = {
                    'name': msg_name,
                    'signals': msg_def.get('signals', {})
                }
        return msg_map

    def decode_message(self, channel_msg: can.Message) -> Dict[str, float]:
        """Decode a CAN message and return signal values"""
        msg_id = channel_msg.arbitration_id
        data = channel_msg.data

        if msg_id not in self.message_map:
            return {}

        msg_info = self.message_map[msg_id]
        signals = msg_info['signals']
        decoded = {}

        # Parse signals from CAN message data
        for signal_name, signal_def in signals.items():
            try:
                # Most signals use simple byte extraction
                if 'start_bit' in signal_def:
                    start_bit = signal_def['start_bit']
                    bits = signal_def.get('bits', 16)
                    byte_offset = start_bit // 8

                    if byte_offset < len(data):
                        raw_value = data[byte_offset]
                        if bits <= 8 and byte_offset < len(data):
                            raw_value = data[byte_offset]
                        elif bits <= 16 and byte_offset + 1 < len(data):
                            if signal_def.get('big_endian', False):
                                raw_value = (data[byte_offset] << 8) | data[byte_offset + 1]
                            else:
                                raw_value = data[byte_offset] | (data[byte_offset + 1] << 8)

                        # Apply scale and offset
                        scale = signal_def.get('scale', 1)
                        offset = signal_def.get('offset', 0)
                        decoded[signal_name] = raw_value * scale + offset
            except (IndexError, KeyError, TypeError):
                pass

        return decoded


class CanDataThread(QObject):
    """Thread for reading live CAN data"""
    data_received = pyqtSignal(dict)
    status_changed = pyqtSignal(str)
    error_occurred = pyqtSignal(str)

    def __init__(self, config_path: Path, interface='pcan', channel='PCAN_USBCH1', bitrate=500000):
        super().__init__()
        self.decoder = CanMessageDecoder(config_path)
        self.interface = interface
        self.channel = channel
        self.bitrate = bitrate
        self.bus = None
        self.running = False
        self.segment_data = {i: SegmentData(i) for i in range(10)}

    def run(self):
        """Connect to CAN bus and read messages"""
        try:
            self.status_changed.emit(f"Connecting to CAN bus ({self.interface})...")

            # Try to connect with specified interface
            try:
                self.bus = can.interface.Bus(
                    interface=self.interface,
                    channel=self.channel,
                    bitrate=self.bitrate,
                    is_fd=False
                )
            except Exception as pcan_error:
                # If PCAN fails, try to get available interfaces
                print(f"Failed to connect with {self.interface}: {pcan_error}")
                available = can.detect_available_interfaces()
                if available:
                    print(f"Available interfaces: {available}")
                    # Try first available interface
                    first_interface = available[0]
                    print(f"Attempting to use: {first_interface}")
                    self.bus = can.interface.Bus(interface=first_interface['interface'],
                                                 channel=first_interface.get('channel', ''),
                                                 bitrate=self.bitrate,
                                                 is_fd=False)
                    self.status_changed.emit(f"Connected! Listening on {first_interface['interface']}")
                else:
                    raise Exception("No CAN interfaces found. Check your PCAN USB connection.")

            self.running = True
            if self.bus:
                self.status_changed.emit(f"Connected! Listening on {self.interface}")

            while self.running:
                try:
                    msg = self.bus.recv(timeout=1.0)
                    if msg is None:
                        continue

                    # Decode CAN message
                    signals = self.decoder.decode_message(msg)
                    if signals:
                        self._parse_segment_data(msg.arbitration_id, signals)
                        # Emit updated segment data
                        for seg_id, seg_data in self.segment_data.items():
                            self.data_received.emit({
                                'segment_id': seg_id,
                                'data': seg_data
                            })
                except Exception as e:
                    print(f"Error processing message: {str(e)}")
                    # Don't spam errors, just continue

        except Exception as e:
            error_msg = f"Failed to connect to CAN bus: {str(e)}"
            print(error_msg)
            self.error_occurred.emit(error_msg)
            self.status_changed.emit("Failed to connect to CAN bus")

    def _parse_segment_data(self, msg_id: int, signals: Dict[str, float]):
        """Parse segment voltage and temperature data from signals"""
        for signal_name, value in signals.items():
            # Parse cell voltage signals: Seg{N}_Cell{M}_Voltage
            if 'Cell' in signal_name and 'Voltage' in signal_name:
                parts = signal_name.split('_')
                seg_idx = int(parts[0][3:])  # Extract N from SegN
                cell_idx = int(parts[1][4:])  # Extract M from CellM

                if seg_idx < 10:
                    self.segment_data[seg_idx].cell_voltages[cell_idx] = float(value)

            # Parse cell temperature signals: Seg{N}_Cell{M}_Temp
            elif 'Cell' in signal_name and 'Temp' in signal_name:
                parts = signal_name.split('_')
                seg_idx = int(parts[0][3:])
                cell_idx = int(parts[1][4:])

                if seg_idx < 10:
                    self.segment_data[seg_idx].cell_temps[cell_idx] = float(value)

    def stop(self):
        """Stop the CAN reader thread"""
        self.running = False
        if self.bus:
            self.bus.shutdown()

