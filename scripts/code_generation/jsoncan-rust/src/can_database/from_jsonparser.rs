use std::collections::HashMap;

use crate::{
    can_database::{
        CanBusModes, CanDatabase, CanMessage, CanNode, JsonRxMsgNames, NodeAlerts, RxMsgs,
        can_alert_gen::generate_node_alert_msgs, error::CanDBError,
        signal_parse::parse_tx_msg_signals,
    },
    parsing::{JsonCanBusMode, JsonCanParser},
};

impl CanDatabase {
    pub fn from(parser: JsonCanParser) -> Result<Self, CanDBError> {
        // figure out forwarding
        for x in &parser.forwarding {
            if x.bus1_name == x.bus2_name {
                return Err(CanDBError::ForwarderReferencesSameBus {
                    forwarder_name: x.forwarder_name.clone(),
                    bus_name: x.bus1_name.clone(),
                });
            }

            let bus_1_node_names: &Vec<String> =
                match &parser.buses.iter().find(|b| b.name == x.bus1_name) {
                    Some(bus) => &bus.node_names,
                    None => {
                        return Err(CanDBError::ForwarderReferencesUndefinedBus {
                            forwarder_name: x.forwarder_name.clone(),
                            bus_name: x.bus1_name.clone(),
                        });
                    }
                };
            if !bus_1_node_names.contains(&x.forwarder_name) {
                return Err(CanDBError::NodeCannotForwardFrom {
                    node_name: x.forwarder_name.clone(),
                    bus_not_on: x.bus1_name.clone(),
                });
            }
            let bus_2_node_names = match &parser.buses.iter().find(|b| b.name == x.bus2_name) {
                Some(bus) => &bus.node_names,
                None => {
                    return Err(CanDBError::ForwarderReferencesUndefinedBus {
                        forwarder_name: x.forwarder_name.clone(),
                        bus_name: x.bus2_name.clone(),
                    });
                }
            };
            if !bus_2_node_names.contains(&x.forwarder_name) {
                return Err(CanDBError::NodeCannotForwardTo {
                    node_name: x.forwarder_name.clone(),
                    bus_not_on: x.bus2_name.clone(),
                });
            }
        }

        // check bus validity
        for bus in &parser.buses {
            if !bus.modes.contains(&bus.default_mode) {
                return Err(CanDBError::BusDefaultModeNotInModes {
                    bus_name: bus.name.clone(),
                    default_mode: bus.default_mode.clone(),
                });
            }

            if parser.buses.iter().filter(|b| b.name == bus.name).count() > 1 {
                return Err(CanDBError::BusNameCollision {
                    bus_name: bus.name.clone(),
                });
            }
        }

        // one can check that all fields of parser are used here
        let mut db = CanDatabase::construct(parser.buses, parser.forwarding, parser.shared_enums)?;

        // stupid ahh lifetime hacks
        let mut rx_msg_names_to_resolve: Vec<(String, Vec<String>)> = Vec::new();
        let mut enum_names: HashMap<String, Vec<String>> = db
            .shared_enums
            .iter()
            .map(|e| (e.name.clone(), vec!["shared".into()]))
            .collect();

        // first handle adding nodes and their tx_msgs
        for n in parser.nodes {
            for e in &n.enums {
                enum_names
                    .entry(e.name.clone())
                    .and_modify(|broadcasters| broadcasters.push(n.name.clone()))
                    .or_insert(vec![n.name.clone()]);
            }

            let tx_node_name = n.name.clone(); // lifetime hack
            db.add_node(CanNode {
                rx_msgs_names: match n.rx_msgs {
                    JsonRxMsgNames::All => RxMsgs::All,
                    JsonRxMsgNames::RxMsgs(rx_msgs) => {
                        rx_msg_names_to_resolve.push((n.name.clone(), rx_msgs));
                        RxMsgs::RxMsgs(Vec::new())
                    }
                },
                name: n.name,
                collects_data: n.collects_data,
                alerts: n.alerts.map(|a| NodeAlerts {
                    infos: a.infos.alerts,
                    warnings: a.warnings.alerts,
                    faults: a.faults.alerts,
                    infos_id: a.infos.id,
                    warnings_id: a.warnings.id,
                    faults_id: a.faults.id,
                    infos_count_id: a.infos.count_id,
                    warnings_count_id: a.warnings.count_id,
                    faults_count_id: a.faults.count_id,
                }),
                enums: n.enums,
            });

            // find the node in the list of nodes
            for msg in n.tx_msgs {
                let signals = parse_tx_msg_signals(
                    msg.signals,
                    &db.nodes
                        .iter()
                        .find(|n| n.name == tx_node_name)
                        .unwrap()
                        .enums,
                    &db.shared_enums,
                    &msg.name,
                    &tx_node_name,
                );
                db.add_tx_msg(CanMessage {
                    name: msg.name,
                    id: msg.id,
                    description: msg.description,
                    cycle_time: msg.cycle_time,
                    log_cycle_time: msg.log_cycle_time,
                    telem_cycle_time: msg.telem_cycle_time,
                    tx_node_name: tx_node_name.clone(),
                    modes: match msg.modes {
                        JsonCanBusMode::Some(modes) => CanBusModes::Some(modes),
                        JsonCanBusMode::All => CanBusModes::All,
                    },
                    signals,
                })?;
            }
        }

        // consistency check to ensure uniqueness of enum names across all enum definitions
        for (enum_name, broadcasters) in enum_names {
            if broadcasters.len() > 1 {
                return Err(CanDBError::EnumMultipleDefinitions {
                    enum_name,
                    definition_locs: broadcasters,
                });
            }
        }

        // check that all nodes on busses are valid
        for bus in &db.buses {
            for node_name in &bus.node_names {
                if !db.nodes.iter().any(|n| &n.name == node_name) {
                    return Err(CanDBError::BusReferencesUndefinedNode {
                        bus_name: bus.name.clone(),
                        node_name: node_name.clone(),
                    });
                }
            }
        }

        // resolve explicit rx messages
        for (rx_name, rx_msg_names) in rx_msg_names_to_resolve.into_iter() {
            let rx_msgs: Vec<u32> = rx_msg_names
                .iter()
                .map(|m| {
                    db.get_message_id_by_name(m)
                        .expect(&format!("Message {} not found in database", m))
                })
                .collect();
            if let RxMsgs::RxMsgs(old_rx_msgs) = &mut db
                .nodes
                .iter_mut()
                .find(|n| n.name == rx_name)
                .expect(&format!("Node {} not found in database", rx_name))
                .rx_msgs_names
            {
                *old_rx_msgs = rx_msgs;
            } else {
                panic!(
                    "Node {} was marked as receiving all messages, but is also trying to explicitly receive some messages. This is a code bug probably.",
                    rx_name
                );
            }
        }

        // alert tx msgs
        let alert_tx_msgs: Vec<CanMessage> = db
            .nodes
            .iter()
            .filter_map(|n| {
                n.alerts
                    .as_ref()
                    .map(|alerts| generate_node_alert_msgs(&n.name, alerts))
            })
            .flatten()
            .collect();
        for msg in alert_tx_msgs {
            let id = msg.id;
            let tx_node_name = msg.tx_node_name.clone();
            db.add_tx_msg(msg)?;
            // rx on all nodes
            db.nodes.iter_mut().for_each(|n| {
                if n.name == tx_node_name {return;}
                match &mut n.rx_msgs_names {
                    RxMsgs::All => (),
                    RxMsgs::RxMsgs(rx_msgs) => {
                        assert!(
                            !rx_msgs.contains(&id),
                            "Node {} is trying to receive ALERT message ID {}, but that ID is already in its rx_msgs list. Note that all alerts are already RXd by default",
                            n.name, id
                        );
                        rx_msgs.push(id);
                    }
                }
            });
        }

        Ok(db)
    }
}
