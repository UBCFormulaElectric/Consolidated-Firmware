import os
import unittest

from ...generate_can_from_json import generate_can_from_json

valid_json_folder = os.path.join(
    os.path.dirname(__file__), os.pardir, "json_configs", "valid_json1"
)
dbc_output = os.path.join(os.path.dirname(__file__), "generated", "test.dbc")
node_list = [
    name
    for name in os.listdir(valid_json_folder)
    if os.path.isdir(os.path.join(valid_json_folder, name))
]


class CodegenSmoke(unittest.TestCase):
    def test_codegen(self):
        for node in node_list:
            print(f"Generating for code for {node}")
            node_path = os.path.join(valid_json_folder, node)
            self.assertTrue(os.path.isdir(node_path))
            file_output = os.path.join(os.path.dirname(__file__), "generated", node)
            generate_can_from_json(
                valid_json_folder, dbc_output, False, node, file_output
            )
