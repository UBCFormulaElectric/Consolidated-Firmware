import os
import sys

sys.path.insert(
    0,
    os.path.abspath(
        os.path.join(
            os.path.dirname(__file__), "../../../../scripts/code_generation/"
        )
    ),
)
bus_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../../../../can_bus/quadruna")
)
from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

can_db = JsonCanParser(bus_path).make_database()