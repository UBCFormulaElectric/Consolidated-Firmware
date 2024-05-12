import json
import os
import sys
from pathlib import Path

from interpreter import Interpreter
from lexer import Lexer
from parser import Parser
from ... import Definitions

project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append(project_root)
MOCK_DATA_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"
SIGNAL_DEF_PATH = Path(Definitions.ROOT_DIR) / "mock_data" / "customSignals"

class ExpressionParser:
    """
    Class for handling read, writes for custom signals
    """
    def __init__(self, local_debug=True):
        self.lexer = Lexer() 
        self.interpreter = Interpreter
        self.local_debug = local_debug
        self.signal_dict = {}
        self.mock_signal_database_file = SIGNAL_DEF_PATH / "test_cust_sig.json"

    def parse(self, s):
        tok = self.lexer.generate_tokens(s)
        parser = Parser(tok)
        tree = parser.expression()
        return self.interpreter.visit(tree)
    
    def add_signal(self, signal_name, signal_expression):
        self.signal_dict[signal_name] = signal_expression

    def save_signals(self):
        if self.local_debug:
            with open(self.mock_signal_database_file, 'w') as file:
                json.dump(self.signal_dict, file)

    def read_signals(self):
        with open(self.mock_signal_database_file, 'r') as file:
            self.signal_dict = json.load(file)
