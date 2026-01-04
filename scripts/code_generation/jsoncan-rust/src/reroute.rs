use crate::can_database::{BusForwarder, CanBus, CanDatabase, CanNode, RxMsgNames};
use std::collections::{HashMap, HashSet};

pub struct CanRxConfig {
    map_by_bus: HashMap<String, Vec<u32>>,
    // map_by_msg: HashMap<u32, String>,
}

impl CanRxConfig {
    pub fn new() -> Self {
        CanRxConfig {
            map_by_bus: HashMap::new(),
            // map_by_msg: HashMap::new(),
        }
    }

    pub fn add_msg_to_bus(self: &mut Self, msg_id: u32, bus_name: &String) {
        self.map_by_bus
            .entry(bus_name.clone())
            .or_insert(Vec::new())
            .push(msg_id);
        // self.map_by_msg.insert(msg_id, bus_name);
    }

    pub fn get_all_msgs(self: &Self) -> Vec<u32> {
        let s: HashSet<u32> = self.map_by_bus.values().flatten().cloned().collect();
        s.into_iter().collect()
    }

    pub fn get_all_busses(self: &Self) -> Vec<String> {
        self.map_by_bus.keys().cloned().collect()
    }

    pub fn get_msgs_on_bus(self: &Self, bus_name: &String) -> Vec<u32> {
        if let Some(msgs) = self.map_by_bus.get(bus_name) {
            return msgs.clone();
        }
        Vec::new()
    }
}

pub struct CanTxConfig {
    map_by_bus: HashMap<String, Vec<u32>>,
}

impl CanTxConfig {
    pub fn new() -> Self {
        CanTxConfig {
            map_by_bus: HashMap::new(),
        }
    }

    pub fn add_bus_to_msg(self: &mut Self, msg_id: u32, bus_name: &String) {
        self.map_by_bus
            .entry(bus_name.clone())
            .or_insert(Vec::new())
            .push(msg_id);
    }

    pub fn get_all_msgs(self: &Self) -> Vec<u32> {
        let s: HashSet<u32> = self.map_by_bus.values().flatten().cloned().collect();
        s.into_iter().collect()
    }

    pub fn get_all(self: &Self) -> Vec<(u32, Vec<String>)> {
        let mut msg_to_busses: HashMap<u32, Vec<String>> = HashMap::new();
        for (bus_name, msg_ids) in &self.map_by_bus {
            for msg_id in msg_ids {
                msg_to_busses
                    .entry(*msg_id)
                    .or_insert(Vec::new())
                    .push(bus_name.clone());
            }
        }
        msg_to_busses.into_iter().collect()
    }
}

pub struct CanForward {
    // config[source_bus][destination_bus] = [msg_id1, msg_id2, ...]
    config: HashMap<String, HashMap<String, Vec<u32>>>,
}

impl CanForward {
    fn new() -> Self {
        CanForward {
            config: HashMap::new(),
        }
    }

    pub fn add_msg_to_forward(self: &mut Self, msg_id: u32, from_bus: &String, to_bus: &String) {
        self.config
            .entry(from_bus.clone())
            .or_insert(HashMap::new())
            .entry(to_bus.clone())
            .or_insert(Vec::new())
            .push(msg_id);
    }

    // returns list of [(msg_id, busses)]
    pub fn get_forwards_for_bus(self: &Self, bus: &String) -> Vec<(u32, Vec<String>)> {
        if let Some(dest_map) = self.config.get(bus) {
            let mut msg_to_busses: HashMap<u32, Vec<String>> = HashMap::new();
            for (dest_bus, msg_ids) in dest_map {
                for msg_id in msg_ids {
                    msg_to_busses
                        .entry(*msg_id)
                        .or_insert(Vec::new())
                        .push(dest_bus.clone());
                }
            }
            return msg_to_busses.into_iter().collect();
        }
        Vec::new()
    }
}

fn _simple_fast_fourier_transform_stochastic_gradient_descent<'cdb>(
    can_db: &'cdb CanDatabase,
    tx_node_name: &String,
    rx_node_name: &String,
) -> Result<(&'cdb String, &'cdb String, Option<&'cdb String>), ()> {
    // difference from the algorithm above is that we don't do a BFS
    // the idea is to only allow the message to be forwarded once
    // so we just need to pick one edge from the adj_list
    // return (source_node, destination_node, rerouter)

    let mut source_nodes: Vec<&CanBus> = can_db
        .buses
        .iter()
        .filter(|b| b.node_names.contains(tx_node_name))
        .collect();
    source_nodes.sort_by(|a, b| a.name.cmp(&b.name));
    let mut destination_nodes: Vec<&CanBus> = can_db
        .buses
        .iter()
        .filter(|b| b.node_names.contains(rx_node_name))
        .collect();
    destination_nodes.sort_by(|a, b| a.name.cmp(&b.name));

    // if we have a direct connection, just return that
    for bus in &source_nodes {
        if destination_nodes.contains(&bus) {
            return Ok((&bus.name, &bus.name, None));
        }
    }

    // if no direct connection, we need to find a forwarding node
    for BusForwarder {
        forwarder_name,
        bus1_name,
        bus2_name,
    } in &can_db.forwarding
    {
        // check if the forwarder is connected to both the source and destination nodes
        if source_nodes.iter().find(|b| b.name == *bus1_name).is_some()
            && destination_nodes
                .iter()
                .find(|b| b.name == *bus2_name)
                .is_some()
        {
            return Ok((bus1_name, bus2_name, Some(forwarder_name)));
        } else if source_nodes.iter().find(|b| b.name == *bus2_name).is_some()
            && destination_nodes
                .iter()
                .find(|b| b.name == *bus1_name)
                .is_some()
        {
            return Ok((bus2_name, bus1_name, Some(forwarder_name)));
        }
    }

    // if no rerouter found, raise an error
    Err(())
}

fn penis_bbl(
    can_db: &CanDatabase,
    rx_node: &CanNode,
    msg_name: &String,
    tx_configs: &mut HashMap<String, CanTxConfig>,
    rx_configs: &mut HashMap<String, CanRxConfig>,
    forwards: &mut HashMap<String, CanForward>,
) {
    let msg = can_db
        .get_message_by_name(msg_name)
        .expect("Message not found in CAN database.");

    let tx_node_name = &msg.tx_node_name;
    let tx_node = can_db
        .nodes
        .iter()
        .find(|n| n.name == *tx_node_name)
        .expect("TX NODE NOT FOUND IN CAN DB");

    // calculate the reroute
    // initial_node_tx_bus, final_node_rx_bus, rerouter_nodes = _fast_fourier_transform_stochastic_gradient_descent(
    //     adj_list, tx_node, rx_node);
    let (initial_node_tx_bus, final_node_rx_bus, reroute_node) =
        _simple_fast_fourier_transform_stochastic_gradient_descent(
            can_db,
            &tx_node.name,
            &rx_node.name,
        )
        .expect("Could not find a route between TX and RX nodes.");

    // process the calculation
    if let Some(rerouter) = reroute_node {
        forwards
            .get_mut(rerouter)
            .expect("Reroute Node not found in CAN database.")
            .add_msg_to_forward(msg.id, &initial_node_tx_bus, &final_node_rx_bus);
    }
    tx_configs
        .get_mut(&tx_node.name)
        .expect("TX Node not found in CAN database.")
        .add_bus_to_msg(msg.id, initial_node_tx_bus);
    rx_configs
        .get_mut(&rx_node.name)
        .expect("RX Node not found in CAN database.")
        .add_msg_to_bus(msg.id, final_node_rx_bus);
}

pub fn resolve_tx_rx_reroute(
    can_db: &CanDatabase,
) -> (
    HashMap<String, CanTxConfig>, // tx config per node
    HashMap<String, CanRxConfig>, // rx config per node
    HashMap<String, CanForward>,  // forwards per node
) {
    let mut tx_configs: HashMap<String, CanTxConfig> = can_db
        .nodes
        .iter()
        .map(|node| (node.name.clone(), CanTxConfig::new()))
        .collect();

    let mut rx_configs: HashMap<String, CanRxConfig> = can_db
        .nodes
        .iter()
        .map(|node| (node.name.clone(), CanRxConfig::new()))
        .collect();

    let mut forwards: HashMap<String, CanForward> = can_db
        .nodes
        .iter()
        .map(|node| (node.name.clone(), CanForward::new()))
        .collect();

    // we want to guarantee that our given topology is a tree
    // we can BFS on the final adjacency list and just make sure
    // there are no cycles
    // adj_list = _build_adj_list(can_db.forwarding, can_db.nodes, can_db.buses)
    // don't need to enforce to be a tree
    // enforce_tree_fermat_little_rsa(adj_list, can_db.buses)

    for rx_node in &can_db.nodes {
        match &rx_node.rx_msgs_names {
            RxMsgNames::All => {
                for msg_name in can_db.get_all_rx_msgs_for(&rx_node.name) {
                    penis_bbl(
                        can_db,
                        rx_node,
                        &msg_name,
                        &mut tx_configs,
                        &mut rx_configs,
                        &mut forwards,
                    );
                }
            }
            RxMsgNames::RxMsgs(m) => {
                for msg_name in m {
                    penis_bbl(
                        can_db,
                        rx_node,
                        msg_name,
                        &mut tx_configs,
                        &mut rx_configs,
                        &mut forwards,
                    );
                }
            }
        }
    }

    return (tx_configs, rx_configs, forwards);
}
