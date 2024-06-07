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
from queue import Queue
from tzlocal import get_localzone

from generated import telem_pb2
from influx_handler import InfluxHandler

logger = logging.getLogger("telemetry_logger")

sys.path.insert(
    0,
    os.path.abspath(
        os.path.join(
            os.path.dirname(__file__), "../../../../scripts/code_generation/"
        )
    ),
)
bus_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../../../../can_bus/quadruna")
)
print("sys.path[0]:", sys.path[0])
print(bus_path)

from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

INVALID_PACKET_SIZES = {255, 165, 253}


class SignalUtil:
    available_signals = {}
    client_signals = {}
    is_setup = False
    signal_df = pd.DataFrame(columns=['time', 'signal', 'value', 'unit'])
    max_df_size = 1  # Define the size threshold


    @classmethod
    def setup(cls, port: str, app):
        cls.ser = serial.Serial(port=port, baudrate=57600, timeout=1)
        cls.ser.reset_input_buffer()
        cls.ser.reset_output_buffer()
        cls.is_setup = True
        cls.app = app

        cls.can_db = JsonCanParser(bus_path).make_database()

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

                packet_size = int.from_bytes(cls.ser.read(1), byteorder="little")
                logger.info(f"Received data: {packet_size}")
                if packet_size in INVALID_PACKET_SIZES:
                    continue

                if (
                    last_bit == 0 and packet_size != 0
                ):  # the size will be different due to 0 not often being include

                    # Read in UART message and parse the protobuf
                    bytes_read = cls.ser.read(packet_size)
                    message_received = telem_pb2.TelemMessage()
                    message_received.ParseFromString(bytes_read)

                    # Make data array out of ints
                    data_array = cls.make_bytes(message_received)

                    # Unpack the data and add the id and meta data
                    signal_list = cls.can_db.unpack(message_received.can_id, data_array)

                    for single_signal in signal_list:

                        # Add the time stamp and get name
                        single_signal["timestamp"] = message_received.time_stamp
                        signal_name = single_signal["name"] 

                        # Update the list of availble signals and add it to client signals
                        if signal_name not in cls.available_signals:
                            cls.available_signals[signal_name] = True
                            cls.client_signals[signal_name] = []

                      
                        # Ensure the value is the correct type (convert to float)
                        value = int(single_signal["value"])


                        #print(single_signal) #Un comment to verify getting signals to terminal

                        # Create a DataFrame for the new signal
                        new_signal_df = pd.DataFrame([{
                            "time": pd.Timestamp.now(tz=get_localzone()),#TODO: Make time more accurate in mili since start
                            "value": value,
                            "unit": single_signal["unit"],
                            "signal": single_signal["name"]
                        }])


                        # Filter out empty or all-NA columns before concatenation
                        cls.signal_df = cls.signal_df.dropna(axis=1, how='all')
                        new_signal_df = new_signal_df.dropna(axis=1, how='all')
                        # Concatenate the new signal DataFrame with the existing one
                        cls.signal_df = pd.concat([cls.signal_df, new_signal_df], ignore_index=True)

                
                         # Emit the message
                        if len(cls.signal_df) >= cls.max_df_size:
                            print(cls.signal_df)
                            InfluxHandler.write(
                                 cls.signal_df, measurement='live'
                            )

                            cls.signal_df = pd.DataFrame(columns=['time', 'value', 'unit', 'signal'])
                            time.sleep(1)
                        
                else:
                    last_bit = packet_size

        except Exception as e:
            logger.error("Error receiving/sending proto msg:", e)
        finally:
            cls.ser.close()



    @classmethod    
    def make_bytes(cls, message):
        """
        Make the byte array out of the messages array.
        """
        return bytearray([
            message.message_0, message.message_1, message.message_2, 
            message.message_3, message.message_4, message.message_5, 
            message.message_6, message.message_7
        ]) 