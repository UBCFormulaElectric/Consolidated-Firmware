import os
import shutil
import unittest

from ...generate_can_from_json import generate_can_from_json

valid_json_folder = os.path.abspath("./test/valid_json")
dbc_output = os.path.abspath("./test/codegen/test.dbc")
ecu1_output = os.path.abspath("./test/codegen/ecu1")
ecu2_output = os.path.abspath("./test/codegen/ecu2")


class CodegenSmoke(unittest.TestCase):
    def test_smoke_ECU1(self):
        # delete ecu1_output folder and its contents
        if os.path.exists(ecu1_output):
            shutil.rmtree(ecu1_output)
        if os.path.exists(ecu2_output):
            shutil.rmtree(ecu2_output)
        generate_can_from_json(valid_json_folder, dbc_output, False, "ECU1", ecu1_output)
        generate_can_from_json(valid_json_folder, dbc_output, False, "ECU2", ecu2_output)
