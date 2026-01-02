mod parse_alert;
mod parse_bus;
mod parse_enum;
mod parse_rx;
mod parse_tx;

use crate::can_database::{BusForwarder, CanAlert, CanBus, CanEnum, RxMsgNames};

use parse_alert::parse_alert_data;
use parse_bus::parse_bus_data;
use parse_enum::{parse_node_enum_data, parse_shared_enums};
use parse_rx::parse_json_rx_data;
pub use parse_tx::JsonTxSignal;
use parse_tx::{JsonCanMessage, parse_tx_data};

struct ParseNode {
    pub name: String,
    pub collects_data: bool,
    pub enums: Vec<CanEnum>,
    pub alerts: Vec<CanAlert>,
    pub tx_msgs: Vec<JsonCanMessage>,
    pub rx_msgs: RxMsgNames,
}

//  A few notes about this class
//     The purpose of this class is to
//     Any functions prefixed with "_parse_" are used to directly parse the JSON data from files
//         - by directly, i mean it pretty much returns exactly what is in the json data without any interpretation
//         - duplication checks are not interpretation
//     Other functions consist the logical representation
pub struct JsonCanParser {
    pub nodes: Vec<ParseNode>,
    pub buses: Vec<CanBus>,
    pub shared_enums: Vec<CanEnum>,
    pub forwarding: Vec<BusForwarder>,
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

impl JsonCanParser {
    /**
     * Parses JSON data
     * can_data_dir: Location of all the json files
     *
     * Note that this function does not do any consistency checking between nodes, busses, messages, etc.
     * This will only happen at the point of the CanDatabase creation, this only tells us that the JSON files are syntactically valid and can be parsed into data structures
     */
    pub fn new(can_data_dir: String) -> Self {
        let node_names: Vec<String> = list_nodes_from_folders(&can_data_dir);
        let (busses, forwarding, loggers) = parse_bus_data(&can_data_dir, &node_names);

        // create node objects for each node
        let nodes: Vec<ParseNode> = node_names
            .iter()
            .map(|node_name| {
                let bus_names_with_node = busses.iter().filter_map(|bus| {
                    if bus.node_names.contains(node_name) {
                        Some(&bus.name)
                    } else {
                        None
                    }
                });
                assert!(
                    bus_names_with_node.count() > 0,
                    "Node '{}' must be on at least one bus",
                    node_name
                );

                let alert_data = parse_alert_data(&can_data_dir, node_name);
                return ParseNode {
                    name: node_name.clone(),
                    collects_data: loggers.contains(node_name),
                    enums: parse_node_enum_data(&can_data_dir, &node_name),
                    alerts: alert_data.unwrap_or_else(|| vec![]),
                    rx_msgs: parse_json_rx_data(&can_data_dir, &node_name),
                    tx_msgs: parse_tx_data(&can_data_dir, &node_name),
                };
            })
            .collect();

        Self {
            nodes,
            buses: busses,
            shared_enums: parse_shared_enums(&can_data_dir),
            forwarding,
        }
    }
}
