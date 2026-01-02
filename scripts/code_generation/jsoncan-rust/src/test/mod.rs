use std::collections::HashMap;
use std::hash::Hash;

use crate::{
    can_database::{CanDatabase, error::CanDBError},
    parsing::JsonCanParser,
};

fn setup() -> Result<CanDatabase, CanDBError> {
    let _can_data_dir = std::path::Path::new(file!()).parent().unwrap();
    CanDatabase::from(JsonCanParser::new(format!(
        "{}/json_configs/valid_json1",
        _can_data_dir.display()
    )))
}

fn assert_setequal<S, T, I1, I2>(iter1: I1, iter2: I2) -> bool
where
    S: Eq + PartialEq + Hash,
    T: Eq + PartialEq + Hash,
    I1: Iterator<Item = S>,
    I2: Iterator<Item = T>,
    HashMap<S, i32>: PartialEq<HashMap<T, i32>>,
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
    use crate::can_database::CanDatabase;

    // These tests check that nodes are created correctly.
    use super::{assert_setequal, setup};
    use std::collections::HashSet;

    static ECU1_MSGS: [&str; 6] = [
        "ECU1_BasicSignalTypes",
        "ECU1_DecimalNumbers",
        "ECU1_DbcMatching",
        "ECU1_MinMaxClamping",
        "ECU1_Debug_Only",
        "ECU1_LongMessage",
    ];
    static ECU1_ALERTS: [&str; 6] = [
        "ECU1_Warnings",
        "ECU1_WarningsCounts",
        "ECU1_Faults",
        "ECU1_FaultsCounts",
        "ECU1_Info",
        "ECU1_InfoCounts",
    ];
    // static ECU2_MSGS: [&str; 1] = ["ECU2_BasicSignalTypes"];
    static ECU3_MSGS: [&str; 1] = ["ECU3_TEST"];
    static ECU3_ALERTS: [&str; 6] = [
        "ECU3_Warnings",
        "ECU3_WarningsCounts",
        "ECU3_Faults",
        "ECU3_FaultsCounts",
        "ECU3_Info",
        "ECU3_InfoCounts",
    ];
    static ECU4_MSGS: [&str; 1] = ["ECU4_TEST"];
    static ECU4_ALERTS: [&str; 6] = [
        "ECU4_Warnings",
        "ECU4_WarningsCounts",
        "ECU4_Faults",
        "ECU4_FaultsCounts",
        "ECU4_Info",
        "ECU4_InfoCounts",
    ];
    static ECU5_MSGS: [&str; 1] = ["ECU5_TEST"];
    static ECU5_ALERTS: [&str; 6] = [
        "ECU5_Warnings",
        "ECU5_WarningsCounts",
        "ECU5_Faults",
        "ECU5_FaultsCounts",
        "ECU5_Info",
        "ECU5_InfoCounts",
    ];

    #[test]
    fn test_all_nodes_present() {
        // Checks that ECU1-5 are the only nodes present in the database.
        let cdb = setup().unwrap();
        assert_setequal(
            cdb.nodes.iter().map(|node| node.name.clone()),
            [
                "ECU1".to_string(),
                "ECU2".to_string(),
                "ECU3".to_string(),
                "ECU4".to_string(),
                "ECU5".to_string(),
            ]
            .into_iter(),
        );
    }

    // #[test]
    // fn test_node_on_busses() {
    //     let cdb = setup().unwrap();
    //     assert_setequal(
    //         cdb.nodes
    //             .iter()
    //             .find(|n| n.name == "ECU1")
    //             .unwrap()
    //             .bus_names
    //             .iter(),
    //         ["can1".to_string(), "can3".to_string()].iter(),
    //     );
    //     assert_setequal(
    //         cdb.nodes.get("ECU2").unwrap().bus_names.iter(),
    //         ["can3".to_string()].iter(),
    //     );
    //     assert_setequal(
    //         cdb.nodes.get("ECU3").unwrap().bus_names.iter(),
    //         ["can1".to_string(), "can2".to_string()].iter(),
    //     );
    //     assert_setequal(
    //         cdb.nodes.get("ECU4").unwrap().bus_names.iter(),
    //         ["can1".to_string()].iter(),
    //     );
    //     assert_setequal(
    //         cdb.nodes.get("ECU5").unwrap().bus_names.iter(),
    //         ["can2".to_string()].iter(),
    //     );
    // }

    #[test]
    fn test_all_tx_messages_present() {
        // Checks that all tx/rx messages are present
        let cdb = setup().unwrap();

        assert_setequal(
            cdb.get_message_by_node("ECU1")
                .unwrap()
                .into_iter()
                .map(|s| s.name),
            ECU1_MSGS
                .iter()
                .chain(ECU1_ALERTS.iter())
                .map(|s| s.to_string()),
        );
        assert_setequal(
            cdb.get_message_by_node("ECU3")
                .unwrap()
                .into_iter()
                .map(|s| s.name),
            ECU3_MSGS
                .iter()
                .chain(ECU3_ALERTS.iter())
                .map(|s| s.to_string()),
        );
        assert_setequal(
            cdb.get_message_by_node("ECU4")
                .unwrap()
                .into_iter()
                .map(|s| s.name),
            ECU4_MSGS
                .iter()
                .chain(ECU4_ALERTS.iter())
                .map(|s| s.to_string()),
        );
        assert_setequal(
            cdb.get_message_by_node("ECU5")
                .unwrap()
                .into_iter()
                .map(|s| s.name),
            ECU5_MSGS
                .iter()
                .chain(ECU5_ALERTS.iter())
                .map(|s| s.to_string()),
        );
    }
}

#[cfg(test)]
mod bus_tests {
    use super::*;

    #[test]
    fn teset_busses_present() {
        let cdb = setup().unwrap();
        assert_setequal(
            cdb.buses.iter().map(|bus| bus.name.clone()),
            ["can1".to_string(), "can2".to_string(), "can3".to_string()].into_iter(),
        );
    }

    #[test]
    fn test_bus1_properties() {
        let cdb = setup().unwrap();
        let can1 = cdb.buses.iter().find(|b| b.name == "can1").unwrap();
        assert_eq!(can1.name, "can1");
        assert_eq!(can1.bus_speed, 4000);
        assert_eq!(can1.fd, true);
        assert_eq!(can1.default_mode, "default");
        assert_setequal(
            can1.modes.iter(),
            ["default".to_string(), "debug".to_string()].iter(),
        );
    }

    #[test]
    fn test_bus2_properties() {
        let cdb = setup().unwrap();
        let can2 = cdb.buses.iter().find(|b| b.name == "can2").unwrap();
        assert_eq!(can2.name, "can2");
        assert_eq!(can2.bus_speed, 1000);
        assert_eq!(can2.fd, false);
        assert_eq!(can2.default_mode, "default");
        assert_setequal(
            can2.modes.iter(),
            ["default".to_string(), "debug".to_string()].iter(),
        );
    }

    #[test]
    fn test_bus3_properties() {
        let cdb = setup().unwrap();
        let can3 = cdb.buses.iter().find(|b| b.name == "can3").unwrap();
        assert_eq!(can3.name, "can3");
        assert_eq!(can3.bus_speed, 500);
        assert_eq!(can3.fd, false);
        assert_eq!(can3.default_mode, "default");
        assert_setequal(
            can3.modes.iter(),
            ["default".to_string(), "debug".to_string()].iter(),
        );
    }
}

mod consistency_check_tests {
    // use super::*;
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
