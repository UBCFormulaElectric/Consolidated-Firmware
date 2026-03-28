use std::collections::HashMap;

use indexmap::IndexMap;

use crate::{
    can_database::{
        CanBusModes, CanDatabase, CanEnum, CanMessage, CanNode, CanSignal, CanSignalType,
        JsonRxMsgNames, NodeAlerts, RxMsgs, can_alert_gen::generate_node_alert_msgs,
        error::CanDBError,
    },
    parsing::{JsonCanBusMode, JsonCanParser, JsonCanSignal},
};

fn calculate_scale_offset(min: f64, max: f64, bits: u16) -> (f64, f64) {
    // return scale, offset
    // ((max - min) / (2f64.powi(bits as i32) - 1.0), min)
    ((max - min) / (1u128 << bits - 1) as f64, min)
}

fn parse_signal(
    signal_name: String,
    signal: JsonCanSignal,
    next_available_bit: u16,
    node_enums: &Vec<CanEnum>,
    shared_enums: &Vec<CanEnum>,
) -> CanSignal {
    // TODO description
    match signal {
        JsonCanSignal::ScaleOffsetBits {
            min,
            max,
            scale,
            offset,
            bits,
            unit,
            start_bit,
            signed,
            big_endian,
            start_value,
        } => CanSignal {
            name: signal_name,
            start_bit: start_bit.unwrap_or(next_available_bit),
            bits,
            scale,
            offset,
            min,
            max,
            start_val: start_value.unwrap_or(min),
            enum_name: None,
            unit,
            signed: signed.unwrap_or(false),
            description: None,
            big_endian: big_endian.unwrap_or(false),
            signal_type: if min == 0f64 && max == 1f64 && scale == 1f64 && offset == 0f64 {
                CanSignalType::Boolean
            } else {
                CanSignalType::Numerical
            },
        },
        JsonCanSignal::BitsMinMax {
            bits,
            min,
            max,
            unit,
            start_bit,
            signed,
            big_endian,
            start_value,
        } => {
            let (scale, offset) = calculate_scale_offset(min, max, bits);
            CanSignal {
                name: signal_name,
                start_bit: start_bit.unwrap_or(next_available_bit),
                bits,
                scale,
                offset,
                min,
                max,
                start_val: start_value.unwrap_or(min),
                enum_name: None,
                unit,
                signed: signed.unwrap_or(false),
                description: None,
                big_endian: big_endian.unwrap_or(false),
                signal_type: if min == 0f64 && max == 1f64 && bits == 1 {
                    CanSignalType::Boolean
                } else {
                    CanSignalType::Numerical
                },
            }
        }
        JsonCanSignal::ResolutionMinMax {
            resolution,
            min,
            max,
            unit,
            start_bit,
            signed,
            big_endian,
            start_value,
        } => CanSignal {
            name: signal_name,
            start_bit: start_bit.unwrap_or(next_available_bit),
            bits: ((max - min) / resolution).log2().ceil() as u16,
            scale: resolution,
            offset: min,
            min,
            max,
            start_val: start_value.unwrap_or(min),
            enum_name: None,
            unit,
            signed: signed.unwrap_or(false),
            description: None,
            big_endian: big_endian.unwrap_or(false),
            signal_type: CanSignalType::Numerical,
        },
        JsonCanSignal::Enum {
            enum_name,
            start_value,
            start_bit,
        } => {
            // notice that we don't use the signal_enum intrinsicly, we just use it's signal properties
            let signal_enum: &CanEnum = node_enums
                .iter()
                .find(|e| e.name == enum_name)
                .or_else(|| shared_enums.iter().find(|e| e.name == enum_name))
                .expect(&format!(
                    "Signal {} references enum '{}' which does not exist.",
                    signal_name, enum_name,
                ));

            CanSignal {
                name: signal_name,
                start_bit: start_bit.unwrap_or(next_available_bit),
                bits: signal_enum.bits(),
                scale: 1.0,
                offset: 0.0,
                min: signal_enum.min_value() as f64,
                max: signal_enum.max_value() as f64,
                start_val: start_value.unwrap_or(0.0),
                enum_name: Some(enum_name),
                unit: None,
                signed: false,
                description: None,
                big_endian: false,
                signal_type: CanSignalType::Enum,
            }
        }
        JsonCanSignal::Bits {
            bits,
            scale,
            unit,
            start_bit,
            signed,
            big_endian,
            start_value,
        } => {
            let (min, max) = match signed {
                Some(true) => (
                    (-1_i128 << (bits - 1)) as f64,
                    ((1_u128 << (bits - 1)) - 1) as f64,
                ),
                Some(false) | None => (0.0, ((1u128 << bits) - 1) as f64),
            };

            CanSignal {
                name: signal_name,
                start_bit: start_bit.unwrap_or(next_available_bit),
                bits,
                scale: scale.unwrap_or(1.0),
                offset: 0.0,
                min,
                max,
                start_val: start_value.unwrap_or(0.0),
                enum_name: None,
                unit,
                signed: signed.unwrap_or(false),
                description: None,
                big_endian: big_endian.unwrap_or(false),
                signal_type: if bits == 1 {
                    CanSignalType::Boolean
                } else {
                    CanSignalType::Numerical
                },
            }
        }
    }
}

fn parse_tx_msg_signals(
    json_signals: IndexMap<String, JsonCanSignal>,
    node_enums: &Vec<CanEnum>,
    shared_enums: &Vec<CanEnum>,
    // also useful
    msg_name: &String,
    tx_node_name: &String,
) -> Vec<CanSignal> {
    static MAX_LEN_BITS: usize = 64 * 8; // 64 bytes

    // Placeholder function to convert JsonTxSignal to CanSignal
    let mut signals: Vec<CanSignal> = Vec::new();
    let mut next_available_bit: u16 = 0;
    let mut occupied_bits: Vec<Option<String>> = vec![None; MAX_LEN_BITS];

    // Parse message signals
    for (signal_name, signal_data) in json_signals {
        if match signal_data {
            JsonCanSignal::ScaleOffsetBits { start_bit, .. }
            | JsonCanSignal::BitsMinMax { start_bit, .. }
            | JsonCanSignal::ResolutionMinMax { start_bit, .. }
            | JsonCanSignal::Enum { start_bit, .. }
            | JsonCanSignal::Bits { start_bit, .. } => start_bit.is_some(),
        } != match signal_data {
            JsonCanSignal::ScaleOffsetBits { start_bit, .. }
            | JsonCanSignal::BitsMinMax { start_bit, .. }
            | JsonCanSignal::ResolutionMinMax { start_bit, .. }
            | JsonCanSignal::Enum { start_bit, .. }
            | JsonCanSignal::Bits { start_bit, .. } => start_bit.is_some(),
        } {
            panic!(
                "In message '{}', either all signals must specify start bits, or none of them should.",
                msg_name
            );
        }

        let signal = parse_signal(
            format!("{}_{}", tx_node_name, signal_name),
            signal_data,
            next_available_bit,
            node_enums,
            shared_enums,
        );

        // Mark a signal's bits as occupied, by inserting the signal's name
        for idx in (signal.start_bit as usize)..((signal.start_bit + signal.bits) as usize) {
            if idx >= MAX_LEN_BITS {
                panic!(
                    "Signal '{}' in '{}' is requesting to put a bit at invalid position {}. Messages have a maximum length of 64 bytes.",
                    signal.name, msg_name, idx
                );
            } else if occupied_bits[idx].is_some() {
                panic!(
                    "Signal '{}' in '{}' is requesting to put a bit at invalid position {}. That position is already occupied by the signal '{}'.",
                    signal.name,
                    msg_name,
                    idx,
                    occupied_bits[idx].as_ref().unwrap()
                );
            }
            occupied_bits[idx] = Some(signal.name.clone());
        }

        next_available_bit += signal.bits;
        signals.push(signal);
    }

    signals
}

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
