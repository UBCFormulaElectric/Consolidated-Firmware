mod parse_alert;
mod parse_bus;
mod parse_enum;
pub mod parse_error;
mod parse_rx;
mod parse_tx;

use crate::can_database::{
    BusForwarder, CanAlert, CanBus, CanEnum, CanMessage, CanNode, RxMsgNames,
};
use std::collections::{HashMap, HashSet};

use parse_alert::parse_alert_data;
use parse_bus::parse_bus_data;
use parse_enum::{parse_node_enum_data, parse_shared_enums};
pub use parse_error::ParseError;
use parse_rx::parse_json_rx_data;
use parse_tx::parse_tx_data;

//  A few notes about this class
//     The purpose of this class is to
//     Any functions prefixed with "_parse_" are used to directly parse the JSON data from files
//         - by directly, i mean it pretty much returns exactly what is in the json data without any interpretation
//         - duplication checks are not interpretation
//     Other functions consist the logical representation
pub struct JsonCanParser {
    // nodes[node_name] gives metadata for node_name
    pub nodes: HashMap<String, CanNode>,
    // busses[bus_name] gives metadata for bus_name
    pub busses: HashMap<String, CanBus>,
    // _msgs[msg_name] gives metadata for msg_name
    pub msgs: HashMap<String, CanMessage>,
    // _alerts[node_name] = dict[CanAlert, AlertsEntry]
    pub alerts: HashMap<String, Vec<CanAlert>>,
    // _enums[enum_name] gives metadata for enum_name
    pub enums: HashMap<String, CanEnum>,
    // _collects_data[node_name] = True if the node collects data
    pub collects_data: HashMap<String, bool>,
    // _forwarding[bus_name] gives metadata for bus_name
    pub forwarding: Vec<BusForwarder>,
    // _signals_to_msgs[signal_name] gives the metadata for the message that contains the signal
}

// From a specified directory, creates empty CanNode objects for each represented nodes (from folders)
fn list_nodes_from_folders(can_data_dir: &String) -> Vec<String> {
    let mut node_names: Vec<String> = Vec::new();

    let entries = std::fs::read_dir(can_data_dir).unwrap();
    for entry in entries {
        let entry = entry.expect("Failed to read entry"); // surely??
        let path = entry.path();
        if !path.is_dir() {
            eprintln!("Skipping non-directory entry: {:?}", path);
            continue;
        }
        let name = path.file_name().expect("Failed to get file name"); // surely??
        let name_str = name.to_str().expect("Failed to convert OsStr to str"); // surely????
        node_names.push(name_str.to_string());
    }

    node_names
}

// TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
fn add_tx_msg(
    msg: CanMessage,
    tx_node_name: &String,
    // tx_node: &CanNode,
    msgs: &mut HashMap<String, CanMessage>,
    // signal_name_to_msgs: &mut HashMap<String, &CanMessage>,
) -> Result<(), ParseError> {
    // This function registers a new message, transmitted by a node on the bus
    //
    // It
    // 1. adds the msg to the global dump of messages (self._msgs)
    // 2. adds the msg name to the list of messages broadcasted by the given node (self._nodes[node_name].tx_msg_names)
    // 3. adds all the signals into the global dump of signals (self._signals_to_msgs)
    // Note this function expects a valid CanMessage object

    // Check if this message name is a duplicate
    if let Some(dup_msg) = msgs.get(&msg.name) {
        // if msgs.contains_key(&msg.name) {
        return Err(ParseError::DuplicateTxMsgName {
            tx_node_name_1: tx_node_name.clone(),
            tx_node_name_2: dup_msg.tx_node_name.clone(),
            tx_msg_name: msg.name.clone(),
        });
    }

    if let Some(dup_msg) = msgs.values().find(|m| m.id == msg.id) {
        return Err(ParseError::DuplicateTxMsgID {
            tx_msg_name: msg.name.clone(),
            tx_node_name_1: tx_node_name.clone(),
            tx_node_name_2: dup_msg.tx_node_name.clone(),
        });
    }

    // TODO determine if a message is FD compatible when it is routed
    // if msg.requires_fd() && !tx_node.fd {
    //     return Err(ParseError::TxFDUnsupported {
    //         fd_msg_name: msg.name.clone(),
    //         non_fd_node_name: tx_node_name.clone(),
    //     });
    // }

    // register the message with the database of all messages

    // TODO figure out how to find conflicting signal names across messages
    // for signal in msgs.get(&msg.name).unwrap().signals.iter() {
    // register the signal with the database of all signals
    // if let Some(dup_msg) = signal_name_to_msgs.get(&signal.name) {
    //     return Err(ParseError::DuplicateTxSignalName {
    //         signal_name: signal.name.clone(),
    //         msg_name: msg.name.clone(),
    //         msg2_name: dup_msg.name.clone(),
    //     });
    // }
    // TODO learn lifetimes lmao
    // signal_name_to_msgs.insert(signal.name.clone(), msgs.get(&msg.name).unwrap());
    // }
    msgs.insert(msg.name.clone(), msg);

    Ok(())
}

impl JsonCanParser {
    /**
     * Parses JSON data
     * can_data_dir: Location of all the json files
     */
    pub fn new(can_data_dir: String) -> Result<Self, ParseError> {
        let node_names: Vec<String> = list_nodes_from_folders(&can_data_dir);
        let (busses, forwarding, loggers) = parse_bus_data(&can_data_dir, &node_names);
        let collects_data: HashMap<String, bool> = node_names
            .iter()
            .map(|node_name| (node_name.clone(), loggers.contains(&node_name)))
            .collect();

        // create node objects for each node
        let mut nodes: HashMap<String, CanNode> = node_names
            .iter()
            .map(|node_name| {
                let bus_names_with_node: Vec<String> = busses
                    .iter()
                    .filter_map(|(bus_name, bus)| {
                        if bus.node_names.contains(node_name) {
                            Some(bus_name.clone())
                        } else {
                            None
                        }
                    })
                    .collect();

                assert!(
                    bus_names_with_node.len() > 0,
                    "Node '{}' must be on at least one bus",
                    node_name
                );

                return (
                    node_name.clone(),
                    CanNode {
                        name: node_name.clone(),
                        bus_names: bus_names_with_node,
                        rx_msgs_names: RxMsgNames::RxMsgs(HashSet::new()),
                    },
                );
            })
            .collect();

        // PARSE TX JSON DATA
        // collect shared enums outside of loop
        let shared_enums: HashMap<String, CanEnum> = parse_shared_enums(&can_data_dir);
        let mut enums: HashMap<String, CanEnum> = HashMap::new(); // PREVIOUSLY shared_enums.clone(); maybe merge them at end?
        // populate this boy
        let mut msgs: HashMap<String, CanMessage> = HashMap::new();
        // let mut signal_name_to_msgs: HashMap<String, &CanMessage> = HashMap::new();
        for tx_node_name in &node_names {
            let node_enums = parse_node_enum_data(&can_data_dir, &tx_node_name);
            enums.extend(node_enums.clone());
            for tx_msg in parse_tx_data(&can_data_dir, &tx_node_name, &node_enums, &shared_enums) {
                add_tx_msg(tx_msg, &tx_node_name, &mut msgs)?
            }
        }

        for rx_node_name in &node_names {
            let rx_node = nodes.get_mut(rx_node_name).expect("Node not found??");
            match parse_json_rx_data(&can_data_dir, &rx_node_name) {
                RxMsgNames::All => {
                    rx_node.rx_msgs_names = RxMsgNames::All;
                }
                RxMsgNames::RxMsgs(msg_names) => {
                    for msg_name in msg_names {
                        let rx_msg: &CanMessage = match msgs.get(&msg_name) {
                            Some(msg) => msg,
                            None => {
                                return Err(ParseError::RxMsgNotFound {
                                    rx_node_name: rx_node_name.clone(),
                                    rx_msg_name: msg_name,
                                });
                            }
                        };
                        rx_node.add_rx_msg(rx_msg)?
                    }
                }
            }
        }

        // PARSE ALERTS DATA

        let mut alerts: HashMap<String, Vec<CanAlert>> = HashMap::new();
        let mut nodes_alert_msgs: HashMap<String, Vec<CanMessage>> = HashMap::new();
        for tx_node_name in &node_names {
            // Parse ALERTS
            let (node_alert_msgs, node_alerts) = match parse_alert_data(&can_data_dir, tx_node_name)
            {
                Some(data) => data,
                None => continue,
            };
            assert!(node_alert_msgs.len() == 6, "Alert messages should be 6");
            alerts.insert(tx_node_name.clone(), node_alerts);
            nodes_alert_msgs.insert(tx_node_name.clone(), node_alert_msgs);
        }

        let alert_boards = nodes_alert_msgs
            .keys()
            .cloned()
            .collect::<HashSet<String>>();
        for (tx_node_name, alert_msgs) in nodes_alert_msgs {
            for alert_msg in alert_msgs {
                let alert_msg_name = alert_msg.name.clone();
                // register the message with the database of all messages
                add_tx_msg(alert_msg, &tx_node_name, &mut msgs)?;
                let alert_msg: &CanMessage = msgs
                    .get(&alert_msg_name)
                    .expect("literally look at the above line");
                for other_rx_node_name in &alert_boards {
                    if tx_node_name == *other_rx_node_name {
                        continue;
                    }
                    nodes
                        .get_mut(other_rx_node_name)
                        .expect("Node should exist??")
                        .add_rx_msg(alert_msg)?;
                }
            }
        }

        // CONSISTENCY TODO work this in?
        // consistency_check()

        Ok(JsonCanParser {
            nodes,
            busses,
            enums,
            msgs,
            collects_data,
            forwarding,
            alerts,
        })
    }
}
