import os
import unittest

from ...generate_can_from_json import generate_can_from_json

valid_json_folder = os.path.abspath("./test/valid_json")
dbc_output = os.path.abspath("./test/codegen/test.dbc")
ecu1_output = os.path.abspath("./test/codegen/ecu1")
ecu2_output = os.path.abspath("./test/codegen/ecu2")


class CodegenSmoke(unittest.TestCase):
    def test_smoke_ECU1(self):
        generate_can_from_json(valid_json_folder, dbc_output, False, "ECU1", ecu1_output)
        generate_can_from_json(valid_json_folder, dbc_output, False, "ECU1", ecu2_output)
