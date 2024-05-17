import os
import sys
from pathlib import Path

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../../process"))
)
sys.path.append("..")

import definitions
from querying.lexer import Lexer
from querying.parser import Parser
from querying.interpreter import Interpreter
from querying.parse_expression import ExpressionParser

project_root = os.path.dirname(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
)
sys.path.append(project_root)
MOCK_DATA_PATH = Path(definitions.ROOT_DIR) / "mock_data" / "data" / "sample_data3.csv"
SIGNAL_DEF_PATH = Path(definitions.ROOT_DIR) / "mock_data" / "customSignals"


class TestParseExpression:
    @staticmethod
    def evaluate_expression(expression):
        """

        :param expression:
        :return:
        """
        lexer = Lexer()

        tokens = lexer.generate_tokens(expression)
        parser = Parser(tokens)
        tree = parser.expression()
        interpreter = Interpreter()
        return interpreter.visit(tree)

    @staticmethod
    def test_create_file():
        expParse = ExpressionParser()
        s = "5 + 3 - SIG1"
        expParse.add_signal("sig1", s)
        expParse.save_signals()
        file_path = (
            Path(definitions.ROOT_DIR)
            / "mock_data"
            / "customSignals"
            / "test_cust_sig.json"
        )
        assert os.path.exists(file_path)
        if os.path.exists(file_path):
            os.remove(file_path)

    @staticmethod
    def test_read_file():
        expParse = ExpressionParser()
        s = "5 + 3 - SIG1"
        expParse.add_signal("sig1", s)
        expParse.save_signals()
        file_path = (
            Path(definitions.ROOT_DIR)
            / "mock_data"
            / "customSignals"
            / "test_cust_sig.json"
        )
        assert os.path.exists(file_path)
        exprParse_2 = ExpressionParser()
        exprParse_2.read_signals()
        print(exprParse_2.signal_dict)
        assert exprParse_2.signal_dict["sig1"] == s
        if os.path.exists(file_path):
            os.remove(file_path)
