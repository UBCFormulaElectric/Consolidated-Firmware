import os
import sys
from pathlib import Path
import json

import Definitions 

project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append(project_root)
MOCK_DATA_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"
SIGNAL_DEF_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "customSignals"

class ExpressionParser:
    def parse(s):
        # Convert to some expression tree to evaluate
        pass

    def save_signal(signal_dict, file_name):
        with open(SIGNAL_DEF_PATH / file_name, 'w') as file:
            json.dump(signal_dict, file)

    def read_signal(file_name):
        full_file_path = SIGNAL_DEF_PATH / file_name
        signal_dict = None
        with open(full_file_path, 'r') as file:
            signal_dict = json.load(file)
        return signal_dict
