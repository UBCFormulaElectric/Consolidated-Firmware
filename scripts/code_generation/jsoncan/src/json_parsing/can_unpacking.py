
import sys
import os
from pathlib import Path

from src.json_parsing.json_can_parsing import JsonCanParser

# sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../../../scripts/code_generation/')))

class PythonCanUtils:

    """
    Class that handles the unpacking of CAN signals for telemetry.
    """

    def unpack_message(self, message_id, raw_data, vs_can_db):
        """
        Take the id and the raw data array of the CAN signal and return a dictionary with unpacked values of that ID
        """
        message = vs_can_db.msgs_for_id(message_id)
        if message is None:
            print(f"No message found with ID {message_id}")
            return

        # Initialize dictionary to store decoded signal values
        decoded_values = {}
        signal_list = message.iloc[0]['signals']


        # Iterate over signals in the message
        for signal in signal_list:
            # print(signal)
            # Extract the 'meta data' from the can data base
            start_byte, start_bit = divmod(signal['start_bit'], 8)
            end_byte, end_bit = divmod(signal['start_bit'] + signal['bits'] - 1, 8)
            signal_bytes = raw_data[start_byte:end_byte + 1]

            extracted_bits = self.extract_bits(signal_bytes, start_bit)

            # Apply scaling and offset to get signal value
            raw_value = extracted_bits * signal['scale'] + signal['offset']

            # Putting into a dictionary to be returned
            decoded_values[signal['name']] = raw_value
        
        return decoded_values

    
    def extract_bits(self, signal_bytes, start_bit):
        """
        Extract bits from signal bytes and return these bits    
        """
        byte_mask = 0xFF
        extracted_bits = 0
        for i, byte in enumerate(signal_bytes):
            bits = byte & byte_mask 
            shifted_bits = self.unpack_left(bits, byte_mask,8*i)
            extracted_bits |= shifted_bits

        extracted_bits >>= start_bit

        return extracted_bits

    def unpack_left(self, input, mask,shift):
        """
        Unpack left
        """
        return (input & mask) << shift
    
    def unpack_right(self,input, mask , shift):
        """
        Unpack right
        """
        return (input & mask ) >> shift
    
    def make_array (self,single_row,data_columns):
        return single_row[data_columns].values

    
import csv
import pandas as pd
import glob

def main():
    # Find path to can_bus jsons
    # path_to_json = os.path.abspath("../../../../can_bus/quadruna")
    # # Find all files that have .json ending
    # file_pattern = os.path.join(path_to_json, "*.json")
    # files = glob.glob(file_pattern)

    # # Output file
    # output_file = "../
    json_can_parser = JsonCanParser(can_data_dir='can_bus/quadruna')
    can_db = json_can_parser.make_database()
    can_putils = PythonCanUtils()
    test = [0]*8
    with open('/home/linux/Documents/Consolidated-Firmware/software/tracksight/backend/app/data1.csv', 'r', newline='') as csvfile:
        test_data= pd.read_csv(csvfile)   
        for index, row in test_data.iterrows():
            # row variable is a list that represents a row in csv

            data_columns = ['Data_0', 'Data_1', 'Data_2', 'Data_3', 'Data_4', 'Data_5', 'Data_6', 'Data_7']
            data_array = can_putils.make_array(row,data_columns)
            print(can_putils.unpack_message(row['ID'],data_array,can_db))

if __name__ == "__main__":
    main()
