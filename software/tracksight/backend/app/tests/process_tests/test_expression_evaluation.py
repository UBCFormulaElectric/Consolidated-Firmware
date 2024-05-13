import os
import sys

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../../process"))
)

from querying.lexer import Lexer
from querying.parser import Parser
from querying.interpreter import Interpreter


def evaluate_expression(expression):
    lexer = Lexer()

    tokens = lexer.generate_tokens(expression)
    parser = Parser(tokens)
    tree = parser.expression()
    interpreter = Interpreter()
    return interpreter.visit(tree)


def test_simple_addition():
    assert evaluate_expression("5 + 3") == 8


def test_combined_operations():
    assert evaluate_expression("5 + 3 - 2") == 6
