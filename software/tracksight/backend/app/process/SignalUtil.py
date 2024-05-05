#This file stores messages received over radio into a data frame so that the front end can access it and display it.

import pandas as pd
import os
import sys
from pathlib import Path
import serial
import json 
#need to change below to telem.proto file
import telem2_pb2

DEBUG_SIZE_MSG_BUF_SIZE = 1
#Dont worry abt rn i am changing things
# project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
# sys.path.append(project_root)
# import Definitions 
# MOCK_DATA_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"

class SignalUtil:
    def __init__(self):
        # Load the lookup table from a JSON file, get path of json
        with open('software/tracksight/backend/app/process/lookup.json', 'r') as file:
            self.lookup_table = json.load(file) # Parses the JSON into a Python dictionary

        self.df = pd.DataFrame(columns=['can_id', 'data', 'time_stamp', 'description', 'unit'])
        self.ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1)
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()

        try:
            while True:
                packet_size = int.from_bytes(self.ser.read(1), byteorder="little")
                bytes = self.ser.read(packet_size)
                message_received = telem2_pb2.TelemMessage()
                message_received.ParseFromString(bytes)

                # Lookup the message ID in the table
                msg_details = self.lookup_message_details(message_received.can_id)

                # Append data along with description and unit from the lookup table
                self.df = pd.concat([self.df, pd.DataFrame([{
                    'can_id': message_received.can_id,
                    'data': message_received.data,
                    'time_stamp': message_received.time_stamp,
                    'description': msg_details['description'], #Retrieve key value pairs from lookup
                    'unit': msg_details['unit'] #Retrieve key value pairs from lookup
                }], columns=self.df.columns)], ignore_index=True)

        except KeyboardInterrupt:
            self.ser.close()
        except Exception as e:
            print("Error receiving/sending proto msg:", e)


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
    # returns all signals in data
    def get_all_signals(self):
         return self.df.copy()

    # return target signal
    def get_signal(self, can_id):
        return self.df[self.df['can_id'] == can_id] #Searches can_id collum and returns that row

    # def get_signal(self, s_id):
    #     return self.df[s_id]


    
