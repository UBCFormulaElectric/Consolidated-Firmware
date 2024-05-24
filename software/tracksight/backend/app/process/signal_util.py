import os
import sys
from pathlib import Path
import pandas as pd

from . import definitions
from . import telem_pb2

import serial 
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../../scripts/code_generation/')))
bus_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../../can_bus/quadruna'))
from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

class SignalUtil:
    """
    SignalUtil class for handling signals
    """

    def __init__(self, is_mock=False):
        self.df = pd.DataFrame(columns=['can_id', 'data', 'time_stamp', 'description', 'unit'])
        self.ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1)
        self.can_db = JsonCanParser(bus_path).make_database()

        self.available_signals = []
        self.client_signals = []

        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
    
    def read_messages(self):
        s = set()
        s.add(255)
        s.add(165)
        #s.add(0)
        s.add(253)
        last_bit = 0

        try:
            while True:
                packet_size = int.from_bytes(self.ser.read(1), byteorder="little")

                if last_bit == 0 and packet_size !=0: #the size will be different due to 0 not often being include
                    
                    # Read in UART message and parse the protobuf
                    bytes = self.ser.read(packet_size)
                    message_received = telem_pb2.TelemMessage()
                    message_received.ParseFromString(bytes)

                    # Make data array out of ints
                    print("Message received is ", message_received)
                    data_array = self.make_bytes(message_received)

                    # Unpack the data and add the id and meta data
                    signal_dict = self.can_db.unpack(message_received.can_id), data_array
                    
                    # Add the time stamp
                    signal_dict["timestamp"] = message_received.time_stamp

                    # Append the new dictionary and list, this is done individually
                    self.client_signals.append(signal_dict)
                    self.available_signals.append(signal_dict["name"])

        except KeyboardInterrupt:
            self.ser.close()
        except Exception as e:
            print("Error receiving/sending proto msg:", e)
        finally:
            self.ser.close()


    def lookup_message_details(self, can_id):
        # Traverse the nested dictionary to find the matching message and signal details
        for subsystem, messages in self.lookup_table.items():
            for message_name, message_info in messages.items():
                if message_info['msg_id'] == can_id:
                    signal_info = next(iter(message_info['signals'].values()))
                    return {
                        'description': message_info['description'],
                        'unit': signal_info['unit']
                    }
        return {'description': 'Unknown', 'unit': 'N/A'}  

    def get_available_signals(self):
        """
        Return a list of the availble signals
        """
        return self.available_signals

    def connect_client_to_signal(self, client_id, signal_name):
        """
        Append the signal name to the clients
        """
        if client_id not in self.client_signals:
            self.client_signals[client_id] = []
        if signal_name in self.available_signals and signal_name not in self.client_signals[client_id]:
            self.client_signals[client_id].append(signal_name)

    def connect_client_to_available_signals(self, client_id):
        """
        Connect the client and provide a list of availble signals
        """
        if client_id not in self.client_signals:
            self.client_signals[client_id] = self.available_signals.copy() 


    def disconnect_client_from_signal(self, client_id, signal_name):
        """
        Remove the signal from the client
        """
        if client_id in self.client_signals and signal_name in self.client_signals[client_id]:
            self.client_signals[client_id].remove(signal_name)

    def disconnect_client(self, client_id):
        """
        Delete the client itself
        """
        if client_id in self.client_signals:
            del self.client_signals[client_id]

    # returns all signals in data
    def make_bytes(self, message):
        """
        Make the byte array out of the messages array.
        """
        return bytearray([
            message.message_0, message.message_1, message.message_2, 
            message.message_3, message.message_4, message.message_5, 
            message.message_6, message.message_7
        ]) #TODO: deal with possibly not having a message due to the value being 0