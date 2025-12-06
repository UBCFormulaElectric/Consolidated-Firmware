mod parse_alert;
mod parse_bus;
mod parse_enum;
mod parse_rx;
mod parse_tx;

use crate::can_database::{
    CanAlert, CanBus, CanDatabase, CanEnum, CanMessage, CanNode, RxMsgNames,
};
use std::collections::{HashMap, HashSet};

use parse_bus::parse_bus_data;
use parse_enum::{parse_node_enum_data, parse_shared_enums};
use parse_tx::parse_tx_data;

pub enum ParseError {
    DuplicateTxMsgName {
        tx_node_name: String,
        msg_name: String,
        msg2_name: String,
    },
    DuplicateTxMsgID {
        tx_node_name: String,
        msg_name: String,
        msg2_name: String,
    },
    TxFDUnsupported {
        fd_msg_name: String,
        non_fd_node_name: String,
    },
    DuplicateTxSignalName {
        signal_name: String,
        msg_name: String,
        msg2_name: String,
    },
}

//  A few notes about this class
//     The purpose of this class is to
//     Any functions prefixed with "_parse_" are used to directly parse the JSON data from files
//         - by directly, i mean it pretty much returns exactly what is in the json data without any interpretation
//         - duplication checks are not interpretation
//     Other functions consist the logical representation
pub struct JsonCanParser {
    // class JsonCanParser:
    //     # popoulate these for CanDatabase
    //     _nodes: Dict[str, CanNode]  # _nodes[node_name] gives metadata for node_name
    //     _busses: Dict[str, CanBus]  # _bus_config[bus_name] gives metadata for bus_name
    //     _msgs: Dict[str, CanMessage]  # _msgs[msg_name] gives metadata for msg_name
    //     _alerts: Dict[
    //         str, List[CanAlert]
    //     ]  # _alerts[node_name] = dict[CanAlert, AlertsEntry]
    //     _forwarding: List[BusForwarder]  # _forwarding[bus_name] gives metadata for bus_name
    //     _enums: Dict[str, CanEnum]  # _enums[enum_name] gives metadata for enum_name
    //     _collects_data: Dict[
    //         str, bool
    //     ]  # _collects_data[node_name] = True if the node collects data
    //     _signals_to_msgs: Dict[str, CanMessage]  # _signals_to_msgs[signal_name] gives the metadata for the message that contains the signal
    can_data_dir: String,
    nodes: std::collections::HashMap<String, CanNode>,
    busses: std::collections::HashMap<String, CanBus>,
    msgs: std::collections::HashMap<String, CanMessage>,
    alerts: std::collections::HashMap<String, Vec<CanAlert>>,
    enums: std::collections::HashMap<String, CanEnum>,
    collects_data: std::collections::HashMap<String, bool>,
    signals_to_msgs: std::collections::HashMap<String, CanMessage>,
}

// def list_nodes_from_folders(can_data_dir: str) -> list[str]:
//     """
//     """
//     return [f.name for f in os.scandir(can_data_dir) if f.is_dir()]

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

fn add_rx_msg(msg_name: &String, rx_node_name: &String) {}
//     def _add_rx_msg(self, msg_name: str, rx_node_name: str) -> None:
//         """
//         This function registers a message which is to be received by a node on the bus

//         Note that this function does not know
//         1. which bus the sender will tx will send the message on
//         2. which bus the receiver will rx the message on

//         This function
//         1. enters the current node into the list of nodes that the message is rx'd by
//         2. adds the message name to the list of messages received by the given node
//         """
//         # if we are already listening to all, we don't need to register this specific message
//         # in particular, this is useful for alerts which will blindly make everyone accept them
//         if type(self._nodes[rx_node_name].rx_msgs_names) == AllRxMsgs:
//             return

//         # Check if this message is defined
//         if msg_name not in self._msgs:
//             raise InvalidCanJson(
//                 f"Message '{msg_name}' received by '{rx_node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
//             )

//         rx_msg = self._msgs[msg_name]
//         if rx_msg.tx_node_name == rx_node_name:
//             raise InvalidCanJson(
//                 f"{rx_node_name} cannot both transmit and receive {msg_name}"
//             )

//         # we already check if self._nodes[rx_node_name].rx_msgs_names is AllRxMsgs
//         if msg_name in self._nodes[rx_node_name].rx_msgs_names: # type: ignore
//             raise InvalidCanJson(
//                 f"Message {msg_name} is already registered to be received by node {rx_node_name}"
//             )

//         if rx_msg.requires_fd() and not self._nodes[rx_node_name].fd:
//             raise InvalidCanJson(
//                 f"Message '{msg_name}' is an FD message, but an RX node '{rx_node_name}' isn't FD-capable and so can't receive it!"
//             )

//         # we already check if self._nodes[rx_node_name].rx_msgs_names is AllRxMsgs
//         self._nodes[rx_node_name].rx_msgs_names.add(msg_name) # type: ignore

// TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
fn add_tx_msg(
    msg: CanMessage,
    tx_node_name: &String,
    tx_node: &CanNode,
    msgs: &mut HashMap<String, CanMessage>,
    signal_name_to_msgs: &mut HashMap<String, &CanMessage>,
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
            tx_node_name: tx_node_name.clone(),
            msg_name: msg.name.clone(),
            msg2_name: dup_msg.name.clone(),
        });
    }

    if let Some(dup_msg) = msgs.values().find(|m| m.id == msg.id) {
        return Err(ParseError::DuplicateTxMsgID {
            tx_node_name: tx_node_name.clone(),
            msg_name: msg.name.clone(),
            msg2_name: dup_msg.name.clone(),
        });
    }

    if msg.requires_fd() && !tx_node.fd {
        return Err(ParseError::TxFDUnsupported {
            fd_msg_name: msg.name.clone(),
            non_fd_node_name: tx_node_name.clone(),
        });
    }

    // register the message with the database of all messages
    for signal in msgs.get(&msg.name).unwrap().signals.iter() {
        // register the signal with the database of all signals
        if let Some(dup_msg) = signal_name_to_msgs.get(&signal.name) {
            return Err(ParseError::DuplicateTxSignalName {
                signal_name: signal.name.clone(),
                msg_name: msg.name.clone(),
                msg2_name: dup_msg.name.clone(),
            });
        }
        // TODO learn lifetimes lmao
        signal_name_to_msgs.insert(signal.name.clone(), msgs.get(&msg.name).unwrap());
    }
    msgs.insert(msg.name.clone(), msg);

    Ok(())
}

impl JsonCanParser {
    pub fn new(can_data_dir: String) -> Result<Self, ParseError> {
        // Parses JSON data
        // can_data_dir: Location of all the json files

        let node_names: Vec<String> = list_nodes_from_folders(&can_data_dir);
        // let busses: HashMap<String, CanBus> = HashMap::new();

        // parse the bus config
        let (busses, forwarding, loggers) = parse_bus_data(&can_data_dir, &node_names);

        // let _collects_data = {
        //     node_name: node_name in loggers for node_name in node_names
        // }
        let collects_data: HashMap<String, bool> = node_names
            .iter()
            .map(|node_name| (node_name.clone(), loggers.contains(&node_name)))
            .collect();

        // create node objects for each node
        let nodes: HashMap<String, CanNode> = node_names
            .iter()
            .map(|node_name| {
                let bus_names: Vec<String> = busses
                    .iter()
                    .filter_map(|(bus_name, bus)| {
                        if bus.node_names.contains(node_name) {
                            Some(bus_name.clone())
                        } else {
                            None
                        }
                    })
                    .collect();

                if bus_names.len() == 0 {
                    panic!("Node {} has no bus", node_name);
                }

                let fd: bool = busses
                    .values()
                    .any(|bus| bus.node_names.contains(node_name) && bus.fd);

                return (
                    node_name.clone(),
                    CanNode {
                        name: node_name.clone(),
                        bus_names: bus_names,
                        rx_msgs_names: RxMsgNames::SomeRxMsgs(HashSet::new()),
                        fd: fd,
                    },
                );
            })
            .collect();

        // PARSE TX JSON DATA
        // collect shared enums outside of loop
        let shared_enums: HashMap<String, CanEnum> = parse_shared_enums(&can_data_dir);
        let mut enums: HashMap<String, CanEnum> = shared_enums.clone();
        // populate this boy
        let mut msgs: HashMap<String, CanMessage> = HashMap::new();
        let mut signal_name_to_msgs: HashMap<String, &CanMessage> = HashMap::new();
        for node_name in node_names {
            let node_enums = parse_node_enum_data(&can_data_dir, &node_name);
            enums.extend(node_enums.clone());
            for tx_msg in parse_tx_data(&can_data_dir, &node_name, &node_enums, &shared_enums) {
                if let Err(e) = add_tx_msg(
                    tx_msg,
                    &node_name,
                    nodes
                        .get(&node_name)
                        .expect("Node should exist, we just created it lmao"),
                    &mut msgs,
                    &mut signal_name_to_msgs,
                ) {
                    return Err(e);
                }
            }
        }

        // PARSE RX JSON
        //         for rx_node_name in node_names:
        //             bus_rx_msgs_json = parse_json_rx_data(can_data_dir, rx_node_name)
        //             if type(bus_rx_msgs_json) == str:
        //                 assert bus_rx_msgs_json == "all", "Schema check has failed"
        //                 # if "all" in messages then add all messages on all busses
        //                 self._nodes[rx_node_name].rx_msgs_names = AllRxMsgs()
        //             else:
        //                 assert type(bus_rx_msgs_json) == list, "Schema check has failed"
        //                 for msg_name in bus_rx_msgs_json:
        //                     self._add_rx_msg(msg_name, rx_node_name)

        //         # PARSE ALERTS DATA
        //         self._alerts = {}
        //         alert_msgs: List[CanMessage] = []
        //         for node_name in node_names:
        //             # Parse ALERTS
        //             alerts_json = parse_alert_data(can_data_dir, node_name)
        //             # since they are optional
        //             if alerts_json is None:
        //                 continue
        //             node_alert_msgs, alerts = alerts_json
        //             assert len(node_alert_msgs) == 6, "Alert messages should be 6"
        //             self._alerts[node_name] = alerts

        //             for alert_msg in node_alert_msgs:
        //                 self._add_tx_msg(alert_msg, node_name)  # tx handling
        //             alert_msgs.extend(node_alert_msgs)

        //         # handling alerts rx, note that only other alert broadcasters can receive the alert
        //         for alert_msg in alert_msgs:
        //             for other_rx_node_name in self._alerts.keys():  # rx handling
        //                 # skip the node that transmit the message
        //                 if alert_msg.tx_node_name == other_rx_node_name:
        //                     continue
        //                 self._add_rx_msg(alert_msg.name, other_rx_node_name)

        //         # CONSISTENCY TODO work this in?
        //         # self._consistency_check()

        Ok(JsonCanParser {
            can_data_dir,
            nodes,
            busses,
            enums,
            msgs,
            collects_data,
            signals_to_msgs: signal_name_to_msgs,
            alerts: HashMap::new(),
        })
    }

    pub fn make_database(&self) -> CanDatabase {
        CanDatabase {
            nodes: self.nodes,
            buses: self.busses,
            msgs: self.msgs,
            alerts: self.alerts,
            enums: self.enums,
            collects_data: self.collects_data,
            signals_to_msgs: self.signals_to_msgs,
            forwarding: vec![],
        }
    }
}
