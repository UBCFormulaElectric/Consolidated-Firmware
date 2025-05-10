import unittest
from typing import Set

from ...src.can_database import CanMessage
from .fixture import CDBTests


class NodeTests(CDBTests):
    """
    These tests check that nodes are created correctly.
    """

    def get_messages_from_ecu(self, ecu_name: str) -> Set[str]:
        raw = [msg.name for msg in self.cdb_valid.msgs.values() if msg.tx_node_name == ecu_name]
        setted = set(raw)
        self.assertEqual(len(raw), len(setted))
        return setted

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
        self.assertSetEqual(self.get_messages_from_ecu("ECU1"), self.ecu1_msgs | self.ec1_alerts)
        self.assertSetEqual(self.get_messages_from_ecu("ECU2"), self.ecu2_msgs)
        self.assertSetEqual(self.get_messages_from_ecu("ECU3"), self.ecu3_msgs | self.ecu3_alerts)
        self.assertSetEqual(self.get_messages_from_ecu("ECU4"), self.ecu4_msgs | self.ecu4_alerts)
        self.assertSetEqual(self.get_messages_from_ecu("ECU5"), self.ecu5_msgs | self.ecu5_alerts)


class BusTests(CDBTests):
    def test_busses_present(self):
        self.assertSetEqual(set(self.cdb_valid.buses.keys()), {"can1", "can2", "can3"})

    def test_bus1_properties(self):
        can1 = self.cdb_valid.buses["can1"]
        self.assertEqual(can1.name, "can1")
        self.assertEqual(can1.bus_speed, 4000)
        self.assertEqual(can1.fd, True)
        self.assertEqual(can1.default_mode, "default")
        self.assertSetEqual(set(can1.modes), {"default", "debug"})

    def test_bus2_properties(self):
        can2 = self.cdb_valid.buses["can2"]
        self.assertEqual(can2.name, "can2")
        self.assertEqual(can2.bus_speed, 1000)
        self.assertEqual(can2.fd, False)
        self.assertEqual(can2.default_mode, "default")
        self.assertSetEqual(set(can2.modes), {"default", "debug"})

    def test_bus3_properties(self):
        can3 = self.cdb_valid.buses["can3"]
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
