import unittest
import os

from ...src.json_parsing.json_can_parsing import JsonCanParser
from ...src.can_database import CanDatabase

valid_json_folder = os.path.abspath("./test/valid_json")

# disable test sorting, so that the order of tests is consistent and per given
unittest.TestLoader.sortTestMethodsUsing = None


class NodeTests(unittest.TestCase):
    """
    These tests check that nodes are created correctly.
    """

    @classmethod
    def setUpClass(cls):
        cls.parser_valid: JsonCanParser = JsonCanParser(valid_json_folder)  # slight white box testing situation
        cls.cdb_valid: CanDatabase = cls.parser_valid.make_database()

    def test_all_nodes_present(self):
        """
        Checks that ECU1 and ECU2 are the only nodes present in the database.
        """
        self.assertIn("ECU1", self.cdb_valid.nodes.keys())
        self.assertIn("ECU2", self.cdb_valid.nodes.keys())
        self.assertEqual(len(self.cdb_valid.nodes), 2)

    def test_all_messages_present(self):
        """
        Checks that all tx/rx messages are present
        :return:
        """
        self.assertSetEqual(
            set(self.cdb_valid.nodes["ECU1"].tx_config.list_names()),
            {'ECU1_BasicSignalTypes', 'ECU1_DecimalNumbers', 'ECU1_DbcMatching', 'ECU1_MinMaxClamping', 'ECU1_Warnings',
             'ECU1_Faults', 'ECU1_WarningsCounts', 'ECU1_FaultsCounts', 'ECU1_Info', 'ECU1_InfoCounts'}
        )
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU2"].tx_config.list_names()),
                            {"ECU2_BasicSignalTypes", 'ECU2_Warnings', 'ECU2_Faults', 'ECU2_WarningsCounts',
                             'ECU2_FaultsCounts', 'ECU2_Info', 'ECU2_InfoCounts'})
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU1"].tx_config.list_names()),
                            {"ECU2_BasicSignalTypes", 'ECU2_Warnings', 'ECU2_Faults', 'ECU2_WarningsCounts',
                             'ECU2_FaultsCounts', 'ECU2_Info', 'ECU2_InfoCounts'})
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU2"].tx_config.list_names()), {
            'ECU1_FaultsCounts', 'ECU1_Info', 'ECU1_WarningsCounts', 'ECU1_Faults', 'ECU1_InfoCounts', 'ECU1_Warnings'})

    def test_node_on_busses(self):
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU1"].bus_names), {"bus1", "bus2"})
        self.assertEqual(set(self.cdb_valid.nodes["ECU2"].bus_names), {"bus2"})


class ConsistencyCheckTests(unittest.TestCase):
    def test_check_consistency(self):
        pass


# """
# Checks that the consistency check works.
# """
# self.assertTrue(self.cdb_t1.check_consistency())


if __name__ == '__main__':
    unittest.main()
