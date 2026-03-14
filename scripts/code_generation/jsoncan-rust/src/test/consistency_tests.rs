use std::collections::HashSet;
use crate::can_database::CanSignalType;
use crate::test::setup;

#[test]
fn test_signal_type_mapping() {
    assert_eq!(CanSignalType::Numerical, CanSignalType::from(CanSignalType::Numerical as u32));
    assert_eq!(CanSignalType::Enum, CanSignalType::from(CanSignalType::Enum as u32));
    assert_eq!(CanSignalType::Alert, CanSignalType::from(CanSignalType::Alert as u32));
    assert_eq!(CanSignalType::Boolean, CanSignalType::from(CanSignalType::Boolean as u32));
}

#[test]
fn test_message_list_consistent() {
    let cdb = setup().unwrap();

    for msg in cdb.get_all_msgs().unwrap() {
        // TODO i'm sure there's more fields to check here
        assert!(
            cdb.nodes
                .iter()
                .find(|n| n.name == msg.tx_node_name)
                .is_some()
        )
    }
}

#[test]
fn test_buses_consistent() {
    // TODO there are more tests to ensure that bus fields are valid

    let cdb = setup().unwrap();

    let bus_count = cdb.buses.len();

    let unique_bus_names = cdb
        .buses
        .iter()
        .map(|b| b.name.clone())
        .collect::<HashSet<String>>()
        .len();
    assert!(
        bus_count == unique_bus_names,
        "There are {} busses but {} unique bus names",
        bus_count,
        unique_bus_names
    );

    let node_names: Vec<String> = cdb.nodes.iter().map(|n| n.name.clone()).collect();
    for bus in cdb.buses {
        for node_name in bus.node_names {
            assert!(node_names.contains(&node_name));
        }
    }
}

#[test]
fn test_c() {
    // node_name_set = set()
    // for node_name, node in self.cdb_valid.nodes.items():
    //     node_name_set.add(node.name)
    //     self.assertEqual(node.name, node_name)
    //     # check if the node is in the bus
    //     for bus_name in node.bus_names:
    //         self.assertTrue(bus_name in self.cdb_valid.buses.keys())
    //         self.assertTrue(node.name in self.cdb_valid.buses[bus_name].node_names)

    //     # check if the rx message are in the msg database
    //     if not isinstance(node.rx_msgs_names, AllRxMsgs):
    //         for msg_name in node.rx_msgs_names:
    //             self.assertTrue(msg_name in self.cdb_valid.msgs.keys())

    // # no overlapping names
    // self.assertEqual(len(node_name_set), len(self.cdb_valid.nodes))
}

#[test]
fn test_forwarders_consistent() {
    let cdb = setup().unwrap();
    let bus_names: Vec<String> = cdb.buses.iter().map(|b| b.name.clone()).collect();
    let node_names: Vec<String> = cdb.nodes.iter().map(|n| n.name.clone()).collect();
    for f in cdb.forwarding {
        assert!(bus_names.contains(&f.bus1_name));
        assert!(bus_names.contains(&f.bus2_name));
        assert!(node_names.contains(&f.forwarder_name));
    }
}

#[test]
fn test_e() {
    // alert_messages: Set[CanMessage] = set()
    // for node_name, alerts in self.cdb_valid.alerts.items():
    //     self.assertTrue(node_name in self.cdb_valid.nodes.keys())
    //     # alerts message has to exist
    //     faults_name = f"{node_name}_Faults"
    //     faults_counts_name = f"{node_name}_FaultsCounts"
    //     warnings_name = f"{node_name}_Warnings"
    //     warnings_counts_name = f"{node_name}_WarningsCounts"
    //     info_name = f"{node_name}_Info"
    //     info_counts_name = f"{node_name}_InfoCounts"

    // self.assertTrue(faults_name in self.cdb_valid.msgs.keys())
    // self.assertTrue(faults_counts_name in self.cdb_valid.msgs.keys())
    // self.assertTrue(warnings_name in self.cdb_valid.msgs.keys())
    // self.assertTrue(warnings_counts_name in self.cdb_valid.msgs.keys())
    // self.assertTrue(info_name in self.cdb_valid.msgs.keys())
    // self.assertTrue(info_counts_name in self.cdb_valid.msgs.keys())

    // fault_message = self.cdb_valid.msgs[faults_name]
    // fault_counts_message = self.cdb_valid.msgs[faults_counts_name]
    // warning_message = self.cdb_valid.msgs[warnings_name]
    // warning_counts_message = self.cdb_valid.msgs[warnings_counts_name]
    // info_message = self.cdb_valid.msgs[info_name]
    // info_counts_message = self.cdb_valid.msgs[info_counts_name]

    // alert_messages.add(fault_message)
    // alert_messages.add(fault_counts_message)
    // alert_messages.add(warning_message)
    // alert_messages.add(warning_counts_message)
    // alert_messages.add(info_message)
    // alert_messages.add(info_counts_message)

    // self.assertEqual(fault_message.tx_node_name, node_name)
    // self.assertEqual(fault_counts_message.tx_node_name, node_name)
    // self.assertEqual(warning_message.tx_node_name, node_name)
    // self.assertEqual(warning_counts_message.tx_node_name, node_name)
    // self.assertEqual(info_message.tx_node_name, node_name)
    // self.assertEqual(info_counts_message.tx_node_name, node_name)
}

#[test]
fn test_f() {
    // # check if the alerts messages are recieived by other nodes
    // for node in self.cdb_valid.nodes.values():
    //     for alert_message in alert_messages:
    //         # skip the message from the node itself
    //         if node.name == alert_message.tx_node_name:
    //             continue

    //         # skip if the node does not have alerts config
    //         if node.name not in self.cdb_valid.alerts.keys():
    //             continue

    //         # skip if the rx is configured as all
    //         if isinstance(node.rx_msgs_names, AllRxMsgs):
    //             continue

    //         self.assertTrue(alert_message.name in node.rx_msgs_names)
}
