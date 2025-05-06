import unittest
import os
from unittest import SkipTest

from ...src.json_parsing.json_can_parsing import JsonCanParser
from ...src.can_database import CanDatabase, CanForward

valid_json_folder = os.path.abspath("./test/valid_json")

# disable test sorting, so that the order of tests is consistent and per given
unittest.TestLoader.sortTestMethodsUsing = None


class CDBTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parser_valid: JsonCanParser = JsonCanParser(valid_json_folder)  # slight white box testing situation
        cls.cdb_valid: CanDatabase = cls.parser_valid.make_database()


class NodeTests(CDBTests):
    """
    These tests check that nodes are created correctly.
    """

    def test_all_nodes_present(self):
        """
        Checks that ECU1-5 are the only nodes present in the database.
        """
        self.assertSetEqual(set(self.cdb_valid.nodes.keys()), {"ECU1", "ECU2", "ECU3", "ECU4", "ECU5"})

    def test_node_on_busses(self):
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU1"].bus_names), {"can1", "can2", "can3"})
        self.assertEqual(set(self.cdb_valid.nodes["ECU2"].bus_names), {"can3"})
        self.assertEqual(set(self.cdb_valid.nodes["ECU3"].bus_names), {"can1", "can2"})
        self.assertEqual(set(self.cdb_valid.nodes["ECU4"].bus_names), {"can1"})
        self.assertEqual(set(self.cdb_valid.nodes["ECU5"].bus_names), {"can2"})

    def test_all_tx_messages_present(self):
        """
        Checks that all tx/rx messages are present
        :return:
        """
        self.assertSetEqual(
            set(self.cdb_valid.nodes["ECU1"].tx_config.list_msg_names()),
            {"ECU1_BasicSignalTypes", "ECU1_DecimalNumbers", "ECU1_DbcMatching", "ECU1_MinMaxClamping", "ECU1_Warnings",
             "ECU1_WarningsCounts", "ECU1_Faults", "ECU1_FaultsCounts", "ECU1_Info", "ECU1_InfoCounts"}
        )
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU2"].tx_config.list_msg_names()),
                            {"ECU2_BasicSignalTypes", "ECU2_Warnings",
                             "ECU2_WarningsCounts", "ECU2_Faults", "ECU2_FaultsCounts", "ECU2_Info", "ECU2_InfoCounts"})
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU3"].tx_config.list_msg_names()),
                            {"ECU3_TEST", "ECU3_Warnings", "ECU3_WarningsCounts", "ECU3_Faults", "ECU3_FaultsCounts",
                             "ECU3_Info", "ECU3_InfoCounts"})
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU4"].tx_config.list_msg_names()),
                            {"ECU4_TEST", "ECU4_Warnings", "ECU4_WarningsCounts", "ECU4_Faults", "ECU4_FaultsCounts",
                             "ECU4_Info", "ECU4_InfoCounts"})
        self.assertSetEqual(set(self.cdb_valid.nodes["ECU5"].tx_config.list_msg_names()),
                            {"ECU5_TEST", "ECU5_Warnings", "ECU5_WarningsCounts", "ECU5_Faults", "ECU5_FaultsCounts",
                             "ECU5_Info", "ECU5_InfoCounts"})

    def test_nonpresent_reroute_configs(self):
        self.assertEqual(self.cdb_valid.nodes["ECU2"].reroute_config, None)
        self.assertEqual(self.cdb_valid.nodes["ECU4"].reroute_config, None)
        self.assertEqual(self.cdb_valid.nodes["ECU5"].reroute_config, None)

    def test_reroute_configs(self):
        ecu1rrc = self.cdb_valid.nodes["ECU1"].reroute_config
        ecu2rrc = self.cdb_valid.nodes["ECU2"].reroute_config
        self.assertSetEqual(set(ecu1rrc),
                            {CanForward("", "", "", ""), CanForward("", "", "", ""), CanForward("", "", "", ""),
                             CanForward("", "", "", ""), CanForward("", "", "", ""), CanForward("", "", "", ""), })
        self.assertSetEqual(set(ecu2rrc),
                            {CanForward("", "", "", ""), CanForward("", "", "", ""), CanForward("", "", "", ""),
                             CanForward("", "", "", ""), CanForward("", "", "", ""), CanForward("", "", "", ""), })


class BusTests(CDBTests):
    def test_busses_present(self):
        self.assertSetEqual(set(self.cdb_valid.busses.keys()), {"can1", "can2", "can3"})

    def test_bus1_properties(self):
        can1 = self.cdb_valid.busses["can1"]
        self.assertEqual(can1.name, "can1")
        self.assertEqual(can1.bus_speed, 4000)
        self.assertEqual(can1.fd, True)
        self.assertEqual(can1.default_mode, "default")
        self.assertSetEqual(set(can1.modes), {"default", "debug"})

    def test_bus2_properties(self):
        can2 = self.cdb_valid.busses["can2"]
        self.assertEqual(can2.name, "can2")
        self.assertEqual(can2.bus_speed, 1000)
        self.assertEqual(can2.fd, False)
        self.assertEqual(can2.default_mode, "default")
        self.assertSetEqual(set(can2.modes), {"default", "debug"})

    def test_bus3_properties(self):
        can3 = self.cdb_valid.busses["can3"]
        self.assertEqual(can3.name, "can3")
        self.assertEqual(can3.bus_speed, 500)
        self.assertEqual(can3.fd, False)
        self.assertEqual(can3.default_mode, "default")
        self.assertSetEqual(set(can3.modes), {"default", "debug"})


class ConsistencyCheckTests(CDBTests):
    def test_check_consistency(self):
        pass


# """
# Checks that the consistency check works.
# """
# self.assertTrue(self.cdb_t1.check_consistency())


if __name__ == "__main__":
    unittest.main()
