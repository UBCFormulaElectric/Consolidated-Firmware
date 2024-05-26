import os
import sys
from pathlib import Path
import pandas as pd
import flask_socketio

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

    def __init__(self, is_modem_reader):

        if(is_modem_reader):
            self.ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1) #TODO: generalize the serial port (only linux as this file structure)
            self.can_db = JsonCanParser(bus_path).make_database()
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()

        self.available_signals = {} #TODO: could just make this the keys
        self.client_signals = {}

        # self.ser.reset_input_buffer()
        # self.ser.reset_output_buffer()
    
    def read_messages(self):
        """
        Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket 
        """
        s = set()
        s.add(255)
        s.add(165)
        s.add(253)
        last_bit = 0

        try:
            while True:
                packet_size = int.from_bytes(self.ser.read(1), byteorder="little")
                print(packet_size)
                if packet_size in s:
                    continue
                

                # if last_bit == 0 and packet_size != 0: #the size will be different due to 0 not often being include
                    
                #     # Read in UART message and parse the protobuf
                #     bytes_read = self.ser.read(packet_size)
                #     message_received = telem_pb2.TelemMessage()
                #     # print(bytes_read)
                #     message_received.ParseFromString(bytes_read)
                #     # Make data array out of ints
                #     # print("Message received is ", message_received)
                #     data_array = self.make_bytes(message_received)

                #     # Unpack the data and add the id and meta data
                #     signal_list = self.can_db.unpack(message_received.can_id, data_array)

                #     for single_signal in signal_list:
                
                #         # Add the time stamp
                #         single_signal["timestamp"] = message_received.time_stamp
                #         signal_name = single_signal["name"]
                #         print(single_signal)

                #         # Update the list of availble signals and add it to client signals
                #         if signal_name not in self.available_signals:
                #             self.available_signals[signal_name] = True
                #             self.client_signals[signal_name] = []
                        
                #         # Emit the message
                #         # flask_socketio.emit('signal_response',single_signal)

                # else: 
                #     last_bit = packet_size

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
        flask_socketio.emit(self.available_signals.copy())

    def connect_client_to_signal(self, client_id, signal_name):
        """
        Append the client_id to a specific signal
        """
        if signal_name in list(self.client_signals.keys()):
            if client_id not in self.client_id[signal_name]:
                self.client_signals[signal_name].append(client_id)

    def connect_client_to_available_signals(self, client_id):
        """
        Connect the client and to all available signals
        """
        for signal_name in list(self.client_signals.keys()):
            if client_id not in self.client_signals[signal_name]:
                self.client_signals[signal_name].append(client_id)


    def disconnect_client_from_signal(self, client_id, signal_name):
        """
        Remove a specific signal from a specific client
        """
        if signal_name in list(self.client_signals.keys()): 
            if client_id in self.client_signals[signal_name]: 
                self.client_signals[signal_name].remove(client_id)


    def disconnect_client(self, client_id):
        """
        Remove a specific client from all signals
        """
        for signal_name in list(self.client_signals.keys()):
            if client_id in self.client_signals[signal_name]:
                self.client_signals[signal_name].remove(client_id)

    def make_bytes(self, message):
        """
        Make the byte array out of the messages array.
        """
        return bytearray([
            message.message_0, message.message_1, message.message_2, 
            message.message_3, message.message_4, message.message_5, 
            message.message_6, message.message_7
        ]) 
        