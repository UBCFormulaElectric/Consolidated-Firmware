"""
Signal util and uart 'handling' class 

Reads in the bytes coming in over UART and emits 
them to the flask app appropriately
TODO: Implement proper error handling for things like no data available.
"""

import os
import sys
import serial
import logging
import time
import pandas as pd
from tzlocal import get_localzone

from generated import telem_pb2
from influx_handler import InfluxHandler

logger = logging.getLogger("telemetry_logger")

sys.path.insert(
    0,
    os.path.abspath(
        os.path.join(
            os.path.dirname(__file__), "../../../../../scripts/code_generation/"
        )
    ),
)
bus_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../../../../../can_bus/quadruna")
)
# from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
# from jsoncan.src.can_database import CanDatabase

VALID_PACKET_SIZES = {255, 165, 253}


class SignalUtil:
    available_signals = {}
    client_signals = {}
    is_setup = False

    @classmethod
    def setup(cls, port: str, app):
        cls.ser = serial.Serial(port=port, baudrate=57600, timeout=1)
        cls.ser.reset_input_buffer()
        cls.ser.reset_output_buffer()
        cls.is_setup = True
        cls.app = app

        # can_db = JsonCanParser(bus_path).make_database()

    @classmethod
    def read_messages(cls):
        """
        Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
        """
        if not cls.is_setup:
            raise RuntimeError("SignalUtil not initialized.")

        last_bit = 0
        try:
            while True:
                # TODO: Lara: Upload actual signals instead!

                # packet_size = int.from_bytes(cls.ser.read(1), byteorder="little")
                # logger.debug(f"Received data: {packet_size}")
                # if packet_size in VALID_PACKET_SIZES:
                #     continue

                # if (
                #     last_bit == 0 and packet_size != 0
                # ):  # the size will be different due to 0 not often being include

                #     # Read in UART message and parse the protobuf
                #     bytes_read = cls.ser.read(packet_size)
                #     message_received = telem_pb2.TelemMessage()
                #     message_received.ParseFromString(bytes_read)

                #     # Make data array out of ints
                #     data_array = cls.make_bytes(message_received)

                #     # Unpack the data and add the id and meta data
                #     signal_list = cls.can_db.unpack(message_received.can_id, data_array)

                #     for single_signal in signal_list:

                #         # Add the time stamp
                #         single_signal["timestamp"] = message_received.time_stamp
                #         signal_name = single_signal["name"]

                #         # Update the list of availble signals and add it to client signals
                #         if signal_name not in cls.available_signals:
                #             cls.available_signals[signal_name] = True
                #             cls.client_signals[signal_name] = []

                #         # Emit the message
                #         flask_socketio.emit("signal_response", single_signal)

                # else:
                #     last_bit = packet_size

                if "Signal" not in cls.available_signals:
                    cls.available_signals["Signal"] = True
                    cls.client_signals["Signal"] = []

                # signal = {
                #     # "timestamp": "2024",
                #     "name": "Signal",
                #     "value": 3,
                #     "unit": "W",
                # }
                # with cls.app.app_context():

                signals = {"time": [], "signal": [], "value": [], "unit": []}
                for _ in range(10):
                    timestamp = pd.Timestamp.now(tz=get_localzone())
                    signals["time"].append(timestamp)
                    signals["signal"].append("Test")
                    signals["value"].append(2)
                    signals["unit"].append("kW")

                InfluxHandler.write(
                    pd.DataFrame(data=signals),
                    measurement="live",
                )

                time.sleep(1)

        except Exception as e:
            logger.error("Error receiving/sending proto msg:", e)
        finally:
            cls.ser.close()
