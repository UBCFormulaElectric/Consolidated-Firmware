import random

from ...src.can_database import CanDatabase
from ...src.codegen.c_generation.routing import CanForward, resolve_tx_rx_reroute
from .fixture import CDBTests


class RoutingTests(CDBTests):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.tx_config, cls.rx_config, cls.reroute_config = resolve_tx_rx_reroute(
            cls.cdb_valid
        )

    def test_nonpresent_reroute_configs(self):
        self.assertEqual(self.reroute_config.get("ECU2"), None)
        self.assertEqual(self.reroute_config.get("ECU4"), None)
        self.assertEqual(self.reroute_config.get("ECU5"), None)

    def test_reroute_configs(self):
        ecu1rrc = self.reroute_config["ECU1"]
        ecu3rrc = self.reroute_config["ECU3"]
        self.assertSetEqual(
            set(ecu1rrc),
            set(
                [
                    CanForward(msg_name, "ECU1", "can3", "can1")
                    for msg_name in self.ecu2_msgs
                ]
            ),
        )
        self.assertSetEqual(
            set(ecu3rrc),
            set(
                [
                    CanForward(msg_name, "ECU3", "can2", "can1")
                    for msg_name in self.ecu5_msgs | self.ecu5_alerts
                ]
            )
            | set(
                [
                    CanForward(msg_name, "ECU3", "can1", "can2")
                    for msg_name in self.ecu4_alerts
                ]
            ),
        )

    def _shuffle_dict(self, d):
        items = list(d.items())
        random.shuffle(items)
        return dict(items)

    def test_deterministic_reroute(self):
        cdb_copy = CanDatabase(
            nodes=self._shuffle_dict(self.cdb_valid.nodes),
            buses=self._shuffle_dict(self.cdb_valid.buses),
            msgs=self._shuffle_dict(self.cdb_valid.msgs),
            alerts=self._shuffle_dict(self.cdb_valid.alerts),
            enums=self._shuffle_dict(self.cdb_valid.enums),
            collects_data=self._shuffle_dict(self.cdb_valid.collects_data),
            forwarding=self.cdb_valid.forwarding,
        )

        random.shuffle(cdb_copy.forwarding)

        tx_config, rx_config, reroute_config = resolve_tx_rx_reroute(cdb_copy)

        self.assertEqual(reroute_config, self.reroute_config)
        self.assertEqual(tx_config, self.tx_config)
        self.assertEqual(rx_config, self.rx_config)
