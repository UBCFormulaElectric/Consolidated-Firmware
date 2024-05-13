import os
import sys
from pathlib import Path

import pandas as pd

from . import definitions

project_root = os.path.dirname(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
)
sys.path.append(project_root)

MOCK_DATA_PATH = Path(definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"


class SignalUtil:
    """
    SignalUtil class for handling signals
    """

    def __init__(self, is_mock=False):
        if is_mock:
            try:
                self.df = pd.read_csv(MOCK_DATA_PATH)
                self.df = self.df.set_index("date_time")
            except:
                print("Error reading mock data file")
        else:
            pass

    # returns all signals in data
    def get_all_signals(self):
        """

        :return:
        """
        return self.df.iloc[:, 0:]

    # return target signal
    def get_signal(self, s_id):
        """

        :param s_id:
        :return:
        """
        return self.df[s_id]
