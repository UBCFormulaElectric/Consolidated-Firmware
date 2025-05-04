import unittest
import os

from ...src.json_parsing.json_can_parsing import JsonCanParser
from ...src.can_database import CanDatabase

test1_json_folder = os.path.abspath("./test1_json")

# disable test sorting, so that the order of tests is consistent and per given
unittest.TestLoader.sortTestMethodsUsing = None


class SmokeTests(unittest.TestCase):
    def test_jsonparser_create(self):
        """
        Tests that the JsonCanParser can be created with a valid folder.
        """
        try:
            parser = JsonCanParser(test1_json_folder)
            self.assertIsInstance(parser, JsonCanParser)
        except Exception as e:
            self.fail(f"JsonCanParser creation failed with exception: {e}")


class NodeTests(unittest.TestCase):
    """
    These tests check that nodes are created correctly.
    """

    @classmethod
    def setUpClass(cls):
        cls.parser_test1: JsonCanParser = JsonCanParser(test1_json_folder)  # slight white box testing situation
        cls.cdb_t1: CanDatabase = cls.parser_test1.make_database()

    def test_all_nodes_present(self):
        """
        Checks that ECU1 and ECU2 are the only nodes present in the database.
        """
        self.assertIn("ECU1", self.cdb_t1.nodes.keys())
        self.assertIn("ECU2", self.cdb_t1.nodes.keys())
        self.assertEqual(len(self.cdb_t1.nodes), 2)

    def test_all_messages_present(self):
        """
        Checks that all tx/rx messages are present
        :return:
        """
        self.assertSetEqual(
            set(self.cdb_t1.nodes["ECU1"].tx_msg_names),
            {'ECU1_BasicSignalTypes', 'ECU1_DecimalNumbers', 'ECU1_DbcMatching', 'ECU1_MinMaxClamping', 'ECU1_Warnings',
             'ECU1_Faults', 'ECU1_WarningsCounts', 'ECU1_FaultsCounts', 'ECU1_Info', 'ECU1_InfoCounts'}
        )
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU2"].tx_msg_names),
                            {"ECU2_BasicSignalTypes", 'ECU2_Warnings', 'ECU2_Faults', 'ECU2_WarningsCounts',
                             'ECU2_FaultsCounts', 'ECU2_Info', 'ECU2_InfoCounts'})
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU1"].rx_msg_names),
                            {"ECU2_BasicSignalTypes", 'ECU2_Warnings', 'ECU2_Faults', 'ECU2_WarningsCounts',
                             'ECU2_FaultsCounts', 'ECU2_Info', 'ECU2_InfoCounts'})
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU2"].rx_msg_names), {
            'ECU1_FaultsCounts', 'ECU1_Info', 'ECU1_WarningsCounts', 'ECU1_Faults', 'ECU1_InfoCounts', 'ECU1_Warnings'})

    def test_node_on_busses(self):
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU1"].bus_names), {"bus1", "bus2"})
        self.assertEqual(set(self.cdb_t1.nodes["ECU2"].bus_names), {"bus2"})

    def test_alerts(self):
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU1"].alerts), set())  # TODO this is not correct
        self.assertSetEqual(set(self.cdb_t1.nodes["ECU2"].alerts), set())


if __name__ == '__main__':
    unittest.main()
