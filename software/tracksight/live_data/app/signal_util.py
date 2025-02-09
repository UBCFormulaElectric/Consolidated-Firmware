"""
Signal util and uart 'handling' class 

Reads in the bytes coming in over UART and emits 
them to the flask app appropriately
TODO: Implement proper error handling for things like no data available.
"""
import pandas as pd
INVALID_PACKET_SIZES = {255, 165, 253}
class SignalUtil:
    available_signals = {}
    client_signals = {}
    signal_df = pd.DataFrame(columns=['time', 'signal', 'value', 'unit'])
    max_df_size = 1  # Define the size threshold
    @classmethod
    def setup(cls, data_file=None):
        cls.data_file = data_file