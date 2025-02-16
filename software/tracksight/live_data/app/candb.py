import os
import sys

dockerized = os.environ.get("IN_DOCKER_CONTAINER") == "true"
if not dockerized:
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
else:
    bus_path = os.path.abspath(
        os.path.join(os.path.dirname(__file__), "../can_bus/quadruna")
    )

from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

can_db = JsonCanParser(bus_path).make_database()