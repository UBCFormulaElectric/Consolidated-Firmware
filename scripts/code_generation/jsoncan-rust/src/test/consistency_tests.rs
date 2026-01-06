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
