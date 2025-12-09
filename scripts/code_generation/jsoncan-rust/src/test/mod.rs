use std::collections::HashMap;
use std::hash::Hash;

use crate::{can_database::CanDatabase, parsing::JsonCanParser};
fn setup() -> (CanDatabase) {
    let cdb = match JsonCanParser::new("./json_configs/valid_json1".to_string()) {
        Ok(parser) => CanDatabase::from(parser),
        Err(_) => panic!("Failed to create parser"),
    };

    return (cdb);
}

pub fn assert_setequal<T, I1, I2>(iter1: I1, iter2: I2) -> bool
where
    T: Eq + Hash,
    I1: Iterator<Item = T>,
    I2: Iterator<Item = T>,
{
    let mut counts1 = HashMap::new();
    for item in iter1 {
        *counts1.entry(item).or_insert(0) += 1;
    }

    let mut counts2 = HashMap::new();
    for item in iter2 {
        *counts2.entry(item).or_insert(0) += 1;
    }

    counts1 == counts2
}

#[cfg(test)]
mod node_tests {
    // These tests check that nodes are created correctly.
    use super::*;
    use std::collections::HashSet;

    // fn get_message_from_ecu() {}
    //     def get_messages_from_ecu(self, ecu_name: str) -> Set[str]:
    //         raw = [
    //             msg.name
    //             for msg in self.cdb_valid.msgs.values()
    //             if msg.tx_node_name == ecu_name
    //         ]
    //         setted = set(raw)
    //         self.assertEqual(len(raw), len(setted))
    //         return setted

    #[test]
    fn test_all_nodes_present() {
        // Checks that ECU1-5 are the only nodes present in the database.
        let (cdb) = setup();
        assert_setequal(
            cdb.nodes.keys(),
            [
                "ECU1".to_string(),
                "ECU2".to_string(),
                "ECU3".to_string(),
                "ECU4".to_string(),
                "ECU5".to_string(),
            ]
            .iter(),
        );
    }

    #[test]
    fn test_node_on_busses() {
        let (cdb) = setup();
        assert_setequal(
            cdb.nodes.get("ECU1").unwrap().bus_names.iter(),
            ["can1".to_string(), "can3".to_string()].iter(),
        );
        assert_setequal(
            cdb.nodes.get("ECU2").unwrap().bus_names.iter(),
            ["can3".to_string()].iter(),
        );
        assert_setequal(
            cdb.nodes.get("ECU3").unwrap().bus_names.iter(),
            ["can1".to_string(), "can2".to_string()].iter(),
        );
        assert_setequal(
            cdb.nodes.get("ECU4").unwrap().bus_names.iter(),
            ["can1".to_string()].iter(),
        );
        assert_setequal(
            cdb.nodes.get("ECU5").unwrap().bus_names.iter(),
            ["can2".to_string()].iter(),
        );
    }

    #[test]
    fn test_all_tx_messages_present() {
        // Checks that all tx/rx messages are present
        let (cdb) = setup();
    }
    //     def test_all_tx_messages_present(self):
    //         """
    //         :return:
    //         """
    //         self.assertSetEqual(
    //             self.get_messages_from_ecu("ECU1"), self.ecu1_msgs | self.ec1_alerts
    //         )
    //         self.assertSetEqual(self.get_messages_from_ecu("ECU2"), self.ecu2_msgs)
    //         self.assertSetEqual(
    //             self.get_messages_from_ecu("ECU3"), self.ecu3_msgs | self.ecu3_alerts
    //         )
    //         self.assertSetEqual(
    //             self.get_messages_from_ecu("ECU4"), self.ecu4_msgs | self.ecu4_alerts
    //         )
    //         self.assertSetEqual(
    //             self.get_messages_from_ecu("ECU5"), self.ecu5_msgs | self.ecu5_alerts
    //         )
}

#[cfg(test)]
mod bus_tests {
    use super::*;
    //     def test_busses_present(self):
    //         self.assertSetEqual(set(self.cdb_valid.buses.keys()), {"can1", "can2", "can3"})

    //     def test_bus1_properties(self):
    //         can1 = self.cdb_valid.buses["can1"]
    //         self.assertEqual(can1.name, "can1")
    //         self.assertEqual(can1.bus_speed, 4000)
    //         self.assertEqual(can1.fd, True)
    //         self.assertEqual(can1.default_mode, "default")
    //         self.assertSetEqual(set(can1.modes), {"default", "debug"})

    //     def test_bus2_properties(self):
    //         can2 = self.cdb_valid.buses["can2"]
    //         self.assertEqual(can2.name, "can2")
    //         self.assertEqual(can2.bus_speed, 1000)
    //         self.assertEqual(can2.fd, False)
    //         self.assertEqual(can2.default_mode, "default")
    //         self.assertSetEqual(set(can2.modes), {"default", "debug"})

    //     def test_bus3_properties(self):
    //         can3 = self.cdb_valid.buses["can3"]
    //         self.assertEqual(can3.name, "can3")
    //         self.assertEqual(can3.bus_speed, 500)
    //         self.assertEqual(can3.fd, False)
    //         self.assertEqual(can3.default_mode, "default")
    //         self.assertSetEqual(set(can3.modes), {"default", "debug"})
}

mod consistency_check_tests {
    use super::*;
    //     def test_check_consistency(self):
    //         # check forign keys for each objects
    //         # check if the name or id are overlapping

    //         msg_id_set = set()
    //         msg_name_set = set()
    //         for msg_name, msg in self.cdb_valid.msgs.items():
    //             self.assertEqual(msg.name, msg_name)
    //             self.assertTrue(msg.tx_node_name in self.cdb_valid.nodes.keys())
    //             msg_id_set.add(msg.id)
    //             msg_name_set.add(msg.name)
    //         self.assertEqual(len(msg_id_set), len(self.cdb_valid.msgs))
    //         self.assertEqual(len(msg_name_set), len(self.cdb_valid.msgs))

    //         bus_name_set = set()
    //         for bus_name, bus in self.cdb_valid.buses.items():
    //             self.assertEqual(bus.name, bus_name)
    //             bus_name_set.add(bus.name)
    //             for node_name in bus.node_names:
    //                 self.assertTrue(node_name in self.cdb_valid.nodes.keys())
    //                 self.assertTrue(bus.name in self.cdb_valid.nodes[node_name].bus_names)
    //         self.assertEqual(len(bus_name_set), len(self.cdb_valid.buses))

    //         node_name_set = set()
    //         for node_name, node in self.cdb_valid.nodes.items():
    //             node_name_set.add(node.name)
    //             self.assertEqual(node.name, node_name)
    //             # check if the node is in the bus
    //             for bus_name in node.bus_names:
    //                 self.assertTrue(bus_name in self.cdb_valid.buses.keys())
    //                 self.assertTrue(node.name in self.cdb_valid.buses[bus_name].node_names)

    //             # check if the rx message are in the msg database
    //             if not isinstance(node.rx_msgs_names, AllRxMsgs):
    //                 for msg_name in node.rx_msgs_names:
    //                     self.assertTrue(msg_name in self.cdb_valid.msgs.keys())

    //         # no overlapping names
    //         self.assertEqual(len(node_name_set), len(self.cdb_valid.nodes))

    //         for forward in self.cdb_valid.forwarding:
    //             self.assertTrue(forward.bus1 in self.cdb_valid.buses.keys())
    //             self.assertTrue(forward.bus2 in self.cdb_valid.buses.keys())
    //             self.assertTrue(forward.forwarder in self.cdb_valid.nodes.keys())

    //         alert_messages: Set[CanMessage] = set()
    //         for node_name, alerts in self.cdb_valid.alerts.items():
    //             self.assertTrue(node_name in self.cdb_valid.nodes.keys())
    //             # alerts message has to exist
    //             faults_name = f"{node_name}_Faults"
    //             faults_counts_name = f"{node_name}_FaultsCounts"
    //             warnings_name = f"{node_name}_Warnings"
    //             warnings_counts_name = f"{node_name}_WarningsCounts"
    //             info_name = f"{node_name}_Info"
    //             info_counts_name = f"{node_name}_InfoCounts"

    //             self.assertTrue(faults_name in self.cdb_valid.msgs.keys())
    //             self.assertTrue(faults_counts_name in self.cdb_valid.msgs.keys())
    //             self.assertTrue(warnings_name in self.cdb_valid.msgs.keys())
    //             self.assertTrue(warnings_counts_name in self.cdb_valid.msgs.keys())
    //             self.assertTrue(info_name in self.cdb_valid.msgs.keys())
    //             self.assertTrue(info_counts_name in self.cdb_valid.msgs.keys())

    //             fault_message = self.cdb_valid.msgs[faults_name]
    //             fault_counts_message = self.cdb_valid.msgs[faults_counts_name]
    //             warning_message = self.cdb_valid.msgs[warnings_name]
    //             warning_counts_message = self.cdb_valid.msgs[warnings_counts_name]
    //             info_message = self.cdb_valid.msgs[info_name]
    //             info_counts_message = self.cdb_valid.msgs[info_counts_name]

    //             alert_messages.add(fault_message)
    //             alert_messages.add(fault_counts_message)
    //             alert_messages.add(warning_message)
    //             alert_messages.add(warning_counts_message)
    //             alert_messages.add(info_message)
    //             alert_messages.add(info_counts_message)

    //             self.assertEqual(fault_message.tx_node_name, node_name)
    //             self.assertEqual(fault_counts_message.tx_node_name, node_name)
    //             self.assertEqual(warning_message.tx_node_name, node_name)
    //             self.assertEqual(warning_counts_message.tx_node_name, node_name)
    //             self.assertEqual(info_message.tx_node_name, node_name)
    //             self.assertEqual(info_counts_message.tx_node_name, node_name)

    //         # check if the alerts messages are recieived by other nodes
    //         for node in self.cdb_valid.nodes.values():
    //             for alert_message in alert_messages:
    //                 # skip the message from the node itself
    //                 if node.name == alert_message.tx_node_name:
    //                     continue

    //                 # skip if the node does not have alerts config
    //                 if node.name not in self.cdb_valid.alerts.keys():
    //                     continue

    //                 # skip if the rx is configured as all
    //                 if isinstance(node.rx_msgs_names, AllRxMsgs):
    //                     continue

    //                 self.assertTrue(alert_message.name in node.rx_msgs_names)
}
