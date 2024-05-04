import pandas as pd
import os
import sys
from pathlib import Path
import serial
import sample_pb2

DEBUG_SIZE_MSG_BUF_SIZE = 1
#Dont worry abt rn i am changing things

# project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
# sys.path.append(project_root)
# import Definitions 
# MOCK_DATA_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"



class SignalUtil:
    def __init__(self, is_mock = True):
        ser = serial.Serial("/dev/ttyUSB0", baudrate = 57600, timeout= 1)
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        while True:
            try:  
                self.df = pd.DataFrame(columns=['can_id', 'data', 'time_stamp'])
                dataBuff= [] #temp
                packet_size = int.from_bytes(ser.read(DEBUG_SIZE_MSG_BUF_SIZE), byteorder="little")
               
                reply = sample_pb2.TelemMessage()
                bytes = ser.read(packet_size)
                reply.ParseFromString(bytes)


                dataBuff.append([reply.can_id, reply.data, reply.time_stamp])

                if len(data_buffer) >= 10:  #  every 10 messages,append
                        temp_df = pd.DataFrame(data_buffer, columns=['can_id', 'data', 'time_stamp'])
                        self.df = pd.concat([self.df, temp_df], ignore_index=True)
                        data_buffer = []  
                else:
                    break  
            except KeyboardInterrupt:
                ser.close() 
            except:
                print("Error receiving/sending proto msg")
            
          
                 

    # returns all signals in data
    def get_all_signals(self):
        return self.df.iloc[:, 0:]

    # return target signal
    def get_signal(self, s_id):
        return self.df[s_id]
    

