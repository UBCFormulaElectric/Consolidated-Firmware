import pandas as pd
import os
import sys
from pathlib import Path

project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append(project_root)
import Definitions 
MOCK_DATA_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data.csv"

class SignalUtil:
    def __init__(self, is_mock = True):
        if is_mock:
            try:
                self.df = pd.read_csv(MOCK_DATA_PATH)
                self.df = self.df.set_index("date_time")
            except:
                print("Error reading mock data file")
        else:
            pass

    def get_signals(self, ids):
        valid_ids = list(filter(lambda s_id : s_id in self.df.columns, ids))
        return self.df[valid_ids].to_json()

    def get_signal(self, s_id):
        return self.df[s_id]

    def get_available_signals
