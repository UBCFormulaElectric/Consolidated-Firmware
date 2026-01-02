// THIS IS FOR REGISTERING ALERT MESSAGES
// let alert_boards = nodes_alert_msgs
//     .keys()
//     .cloned()
//     .collect::<HashSet<String>>();
// for (tx_node_name, alert_msgs) in nodes_alert_msgs {
//     for alert_msg in alert_msgs {
//         let alert_msg_name = alert_msg.name.clone();
//         // register the message with the database of all messages
//         add_tx_msg(alert_msg, &tx_node_name, &mut msgs)?;
//         let alert_msg: &CanMessage = msgs
//             .get(&alert_msg_name)
//             .expect("literally look at the above line");
//         for other_rx_node_name in &alert_boards {
//             if tx_node_name == *other_rx_node_name {
//                 continue;
//             }
//             nodes
//                 .get_mut(other_rx_node_name)
//                 .expect("Node should exist??")
//                 .add_rx_msg(alert_msg)?;
//         }
//     }
// }
