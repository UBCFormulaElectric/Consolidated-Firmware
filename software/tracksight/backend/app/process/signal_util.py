import os
import sys

# from pathlib import Path
import serial
import json 
#need to change below to telem.proto file
#from process import telem2_pb2 # TODO: fix this, for when running from telem.py
import telem2_pb2 #For when running signal_util file on own
import sys
import pandas as pd #TODO: replace pd datafram with local influx db server

# Step 1: Determine the absolute path to the base 'Consolidated-Firmware' directory
base_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../../'))
# Step 2: Define the path to the 'json_parsing' directory
json_parsing_dir = os.path.join(base_dir, 'scripts/code_generation/jsoncan/src')
# Step 3: Add the directory to sys.path
if json_parsing_dir not in sys.path:
    sys.path.append(json_parsing_dir)

# Step 4: Now you can import the JsonCanParser
from json_parsing.json_can_parsing import JsonCanParser

DEBUG_SIZE_MSG_BUF_SIZE = 1
class SignalUtil:
    def __init__(self):
        # Load the lookup table from a JSON file, get path of json
        with open('process/lookup.json', 'r') as file:
            self.lookup_table = json.load(file) # Parses the JSON into a Python dictionary #TODO throw a proper error

        self.df = pd.DataFrame(columns=['can_id', 'data', 'time_stamp', 'description', 'unit'])
        self.ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1)
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()

    def read_messages(self):
        s = set()
        s.add(255)
        s.add(165)
        #s.add(0)
        s.add(253)
        last_bit = 0;
        path_to_json= os.path.abspath('../../../can_bus/quadruna')
        can_db = JsonCanParser(path_to_json).make_database()
        # #Get can info
        #info = database.msg_id_info(msg_id)
        try:
            while True:
                packet_size = int.from_bytes(self.ser.read(1), byteorder="little")
                if packet_size in s:
                    continue
                # print(packet_size)
                # continue #some debugging
                if last_bit == 0 and packet_size != 0:
                    bytes = self.ser.read(packet_size)
                    message_received = telem2_pb2.TelemMessage()
                    message_received.ParseFromString(bytes)
                    print("Message received is ", message_received)
                    # Lookup the message ID in the table
                    msg_details = can_db.msg_id_info(message_received.can_id) #Gives you CanMessage object with all the id info

                    # Append data along with description and unit from the lookup table
                    self.df = pd.concat([self.df, pd.DataFrame([{
                        'can_id': msg_details.id, #Get id
                        'data': message_received.message,
                        'time_stamp': message_received.time_stamp,
                        'description': msg_details.description, #Retrieve from CanMessage obj
                        'unit': msg_details['unit'] #Retrieve key value pairs from lookup
                    }], columns=self.df.columns)], ignore_index=True)
                    print(self.df)
                else:
                    last_bit = packet_size
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
        return self.df['description'].copy()

    # return target signal
    def get_signal(self, descriptor):
        return self.df[self.df['description'] == descriptor] #Searches can_id collum and returns that row

    # def get_signal(self, s_id):
    #     return self.df[s_id]
if __name__ == '__main__':
    signals = SignalUtil()
    signals.read_messages()
