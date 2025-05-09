from .fixture import CDBTests
from ...src.codegen.c_generation.routing import CanForward, resolve_tx_rx_reroute


class RoutingTests(CDBTests):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.tx_config, cls.rx_config, cls.reroute_config = resolve_tx_rx_reroute(cls.cdb_valid)

    def test_nonpresent_reroute_configs(self):
        self.assertEqual(self.reroute_config.get("ECU2"), None)
        self.assertEqual(self.reroute_config.get("ECU4"), None)
        self.assertEqual(self.reroute_config.get("ECU5"), None)

    def test_reroute_configs(self):
        ecu1rrc = self.reroute_config["ECU1"]
        ecu3rrc = self.reroute_config["ECU3"]
        self.assertSetEqual(set(ecu1rrc),
                            set([CanForward(msg_name, "ECU1", "can3", "can1") for msg_name in self.ecu2_msgs]))
        self.assertSetEqual(set(ecu3rrc),
                            set([CanForward(msg_name, "ECU3", "can2", "can1") for msg_name in
                                 self.ecu5_msgs | self.ecu5_alerts]) | set(
                                [CanForward(msg_name, "ECU3", "can1", "can2") for msg_name in self.ecu4_alerts]))
