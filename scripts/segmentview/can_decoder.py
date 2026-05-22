"""
CAN message decoding for BMS data
"""

from pathlib import Path
from typing import Dict
import re

import can
import cantools
from PyQt5.QtCore import Qt, pyqtSignal, QObject

from .segments_model import SegmentData


class CanMessageDecoder:
    """Decodes CAN messages from BMS according to hexray.dbc schema"""

    def __init__(self, dbc_path: Path):
        """Initialize decoder with DBC file"""
        self.db = None
        self._load_dbc(dbc_path)

    def _load_dbc(self, dbc_path: Path):
        """Load DBC file using cantools"""
        try:
            self.db = cantools.database.load_file(str(dbc_path), strict=False)
            print(f"Loaded DBC from {dbc_path}")
        except FileNotFoundError:
            print(f"Warning: Could not find DBC file at {dbc_path}")
            self.db = None
        except Exception as e:
            print(f"Error loading DBC file: {e}")
            self.db = None

    def decode_message(self, channel_msg: can.Message) -> Dict[str, float]:
        """Decode a CAN message and return signal values"""
        if self.db is None:
            return {}

        try:
            # Decode the message using cantools
            msg = self.db.get_message_by_frame_id(channel_msg.arbitration_id)
            decoded = msg.decode(channel_msg.data, allow_truncated=True)
            return decoded
        except (KeyError, Exception) as e:
            # Message ID not found or decode error - return empty dict
            return {}


class CanDataThread(QObject):
    """Thread for reading live CAN data"""
    data_received = pyqtSignal(dict)
    status_changed = pyqtSignal(str)
    error_occurred = pyqtSignal(str)

    # Message IDs for BMS segment voltages and temperatures
    RELEVANT_MESSAGE_IDS: set[int] = {
        422,  # BMS_CellVoltages_Seg0_Seg3
        423,  # BMS_CellVoltages_Seg4_Seg7
        424,  # BMS_CellVoltages_Seg8_Seg9
        428,  # BMS_CellTemps_Seg0_Seg3
        429,  # BMS_CellTemps_Seg4_Seg7
        430,  # BMS_CellTemps_Seg8_Seg9
        419,  # BMS_SegmentStats
        431,  # BMS_CellOpenWireCheck
        432,  # BMS_ThermistorOpenWireCheck
        434,  # BMS_SegmentCommOk
        435,  # BMS_SegmentVref2
        436,  # BMS_SegmentITMP
        437,  # BMS_SegmentVD
        438,  # BMS_SegmentVA
        439,  # BMS_SegmentVD
        443,  # BMS_CellOverVoltage
        444,  # BMS_CellUnderVoltage
        445,  # BMS_SegmentVoltages
        440,  # BMS_SegmentVA_OV
        441,  # BMS_SegmentVA_UV
        442,  # BMS_SegmentVD_OV
        443,  # BMS_SegmentVD_UV
        448,  # BMS_SegmentCED
        449,  # BMS_SegmentCMED
        450,  # BMS_SegmentSED
        451,  # BMS_SegmentSMED
        452,  # BMS_SegmentVDE
        453,  # BMS_SegmentVDEL
        454,  # BMS_SegmentTHSD
        455,  # BMS_SegmentTMODCHK
        456,  # BMS_SegmentOSCCHK
    }

    RELEVANT_MESSAGE_NAMES: set[str] = {
        "BMS_CellVoltages_Seg0_Seg3",
        "BMS_CellVoltages_Seg4_Seg7",
        "BMS_CellVoltages_Seg8_Seg9",
        "BMS_CellTemps_Seg0_Seg3",
        "BMS_CellTemps_Seg4_Seg7",
        "BMS_CellTemps_Seg8_Seg9",
        "BMS_SegmentStats",
        "BMS_CellOpenWireCheck",
        "BMS_ThermistorOpenWireCheck",
        "BMS_SegmentCommOk",
        "BMS_SegmentVref2",
        "BMS_SegmentITMP",
        "BMS_SegmentVD",
        "BMS_SegmentVA",
        "BMS_SegmentVD",
        "BMS_CellOverVoltage",
        "BMS_CellUnderVoltage",
        "BMS_SegmentVoltages",
        "BMS_SegmentVA_OV",
        "BMS_SegmentVA_UV",
        "BMS_SegmentVD_OV",
        "BMS_SegmentVD_UV",
        "BMS_SegmentCED",
        "BMS_SegmentCMED",
        "BMS_SegmentSED",
        "BMS_SegmentSMED",
        "BMS_SegmentVDE",
        "BMS_SegmentVDEL",
        "BMS_SegmentTHSD",
        "BMS_SegmentTMODCHK",
        "BMS_SegmentOSCCHK"
    }

    def __init__(self, dbc_path: Path, interface='vector', channel='0', bitrate=1_000_000):
        super().__init__()
        self.decoder = CanMessageDecoder(dbc_path)
        self.interface = interface
        self.channel = channel
        self.bitrate = bitrate
        self.bus = None
        self.running = False
        self.segment_data: dict[int, SegmentData] = {i: SegmentData(i) for i in range(10)}

        self.voltage_signal_name_re = re.compile(r"BMS_Seg(\d+)_Cell(\d+)_Voltage")
        self.temp_signal_name_re = re.compile(r"BMS_Seg(\d+)_Cell(\d+)_Temp")

    def run(self):
        """Connect to CAN bus and read messages"""
        self.status_changed.emit(f"Connecting to CAN bus ({self.interface})...")

        # Try to connect with specified interface
        try:
            fdcan_args = {
                "sjw_abr": 10,
                "tseg1_abr": 29,
                "tseg2_abr": 10,
                "sam_abr": 1,
                "sjw_dbr": 10,
                "tseg1_dbr": 29,
                "tseg2_dbr": 10,
                "output_mode": 1,
            }
            self.bus = can.interface.Bus(
                interface=self.interface,
                channel=self.channel,
                fd=True,
                bitrate=self.bitrate,
                data_bitrate=self.bitrate,
                app_name=None,
                **fdcan_args,
            )
        except Exception as connection_err:
            # If PCAN fails, try to get available interfaces
            print(f"Failed to connect with {self.interface}: {connection_err}")
            available = can.detect_available_interfaces()
            if available:
                print(f"Available interfaces: {available}")
            else:
                print("No CAN interfaces found")
            raise Exception("CAN connection failed.")

        self.running = True
        if self.bus:
            self.status_changed.emit(f"Connected! Listening on {self.interface}")

        # send the Debug_CanMode message to enable debug mode with signal EnableDebugMode
        try:
            debug_msg = can.Message(arbitration_id=1001, data=[1], is_extended_id=False)
            self.bus.send(debug_msg)
            print("Sent Debug_CanMode message to enable debug mode")
        except Exception as e:
            print(f"Failed to send Debug_CanMode message: {e}")

        while self.running:
            try:
                msg: can.Message = self.bus.recv(timeout=1.0)
                if msg is None:
                    continue

                # Filter: only process BMS segment voltage and temperature messages
                if msg.arbitration_id not in self.RELEVANT_MESSAGE_IDS:
                    continue

                # Decode CAN message
                signals: Dict[str, float] = self.decoder.decode_message(msg)
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

    def _parse_segment_data(self, msg_id: int, signals: Dict[str, float]):
        """Parse segment voltage and temperature data from signals

        Handles DBC signal names like:
        - BMS_Seg0_Cell0_Voltage
        - BMS_Seg0_Cell0_Temp
        """
        assert(self.decoder.db is not None)  # Should only be called if DBC is loaded
        msg_name = self.decoder.db.get_message_by_frame_id(msg_id).name

        if msg_name == "BMS_SegmentStats":
            pass
        if msg_name == "BMS_CellOpenWireCheck":
            pass

        if msg_name == "BMS_CellVoltages_Seg0_Seg3" or msg_name == "BMS_CellVoltages_Seg4_Seg7" or msg_name == "BMS_CellVoltages_Seg8_Seg9":
            for signal_name, value in signals.items():
                # Parse cell voltage signals: BMS_SegN_CellM_Voltage
                res = self.voltage_signal_name_re.match(signal_name)
                assert res  # Should only match voltage signals
                seg_idx = int(res.group(1))
                cell_idx = int(res.group(2))
                assert(seg_idx < 10 and cell_idx < 16)  # Sanity check for indices
                self.segment_data[seg_idx].cell_voltages[cell_idx] = float(value)

        if msg_name == "BMS_CellTemps_Seg0_Seg3" or msg_name == "BMS_CellTemps_Seg4_Seg7" or msg_name == "BMS_CellTemps_Seg8_Seg9":
            for signal_name, value in signals.items():
                res = self.temp_signal_name_re.match(signal_name)
                assert res  # Should only match temperature signals
                seg_idx = int(res.group(1))
                cell_idx = int(res.group(2))
                assert(seg_idx < 10 and cell_idx < 16)  # Sanity check for indices
                self.segment_data[seg_idx].cell_temps[cell_idx] = float(value)
                continue

    def stop(self):
        """Stop the CAN reader thread"""
        self.running = False
        if self.bus:
            self.bus.shutdown()
