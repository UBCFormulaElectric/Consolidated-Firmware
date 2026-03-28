use crate::can_database::{BusForwarder, CanBus};

#[derive(serde::Deserialize)]
struct BusForwarderSchema {
    forwarder: String,
    bus1: String,
    bus2: String,
}

#[derive(serde::Deserialize)]
struct BusDataSchema {
    name: String,
    bus_speed: u32,
    modes: Vec<String>,
    default_mode: String,
    nodes: Vec<String>,
    fd: Option<bool>,
}

#[derive(serde::Deserialize)]
struct BusSchema {
    forwarders: Vec<BusForwarderSchema>,
    buses: Vec<BusDataSchema>,
    loggers: Option<Vec<String>>,
}

pub fn parse_bus_data(
    can_data_dir: &String,
    node_names: &Vec<String>,
) -> (Vec<CanBus>, Vec<BusForwarder>, Vec<String>) {
    let file_path = format!("{}/bus.json", can_data_dir);
    let file_content = std::fs::read_to_string(file_path).expect(&format!(
        "Failed to read bus.json in CAN data directory {}",
        can_data_dir
    ));

    let json_bus: BusSchema =
        serde_json::from_str(&file_content).expect("Failed to parse bus.json");

    // dynamic validation of bus data
    let busses: Vec<CanBus> = json_bus
        .buses
        .into_iter()
        .map(|bus| {
            if !bus.modes.contains(&bus.default_mode) {
                panic!(
                    "Error on bus {}: Default CAN mode is not in the list of modes.",
                    bus.name
                );
            }
            for node in &bus.nodes {
                if !node_names.contains(node) {
                    panic!("Node '{}' is not defined in the node JSON.", node);
                }
            }
            CanBus {
                name: bus.name,
                bus_speed: bus.bus_speed,
                modes: bus.modes,
                default_mode: bus.default_mode,
                node_names: bus.nodes,
                fd: bus.fd.unwrap_or(false),
            }
        })
        .collect();

    let forwarders: Vec<BusForwarder> = json_bus
        .forwarders
        .into_iter()
        .map(|forwarder| {
            if !busses.iter().any(|bus| bus.name == forwarder.bus1) {
                panic!("Bus '{}' is not defined in the bus JSON.", forwarder.bus1);
            }
            if !busses.iter().any(|bus| bus.name == forwarder.bus2) {
                panic!("Bus '{}' is not defined in the bus JSON.", forwarder.bus2);
            }
            if !node_names.contains(&forwarder.forwarder) {
                panic!(
                    "Forwarder '{}' is not defined in the node JSON.",
                    forwarder.forwarder
                );
            }
            BusForwarder {
                bus1_name: forwarder.bus1,
                bus2_name: forwarder.bus2,
                forwarder_name: forwarder.forwarder,
            }
        })
        .collect();

    let logger_node_names = match json_bus.loggers {
        Some(logger_list) => {
            for logger in &logger_list {
                if !node_names.contains(logger) {
                    panic!("Logger '{}' is not defined in the node JSON.", logger);
                }
            }
            logger_list
        }
        None => Vec::new(),
    };

    return (busses, forwarders, logger_node_names);
}
