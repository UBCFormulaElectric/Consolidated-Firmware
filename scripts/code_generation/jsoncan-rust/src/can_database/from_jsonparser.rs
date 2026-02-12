use std::collections::HashMap;

use crate::{
    can_database::{
        error::CanDBError, CanAlert, CanAlertType, CanDatabase, CanEnum, CanMessage, CanNode,
        CanSignal, CanSignalType, GroupedAlerts, JsonRxMsgNames
    },
    parsing::{JsonCanParser, JsonCanSignal, DEFAULT_BUS_MODE},
};
use crate::can_database::RxMsgs;

fn calculate_scale_offset(min: f64, max: f64, bits: u16) -> (f64, f64) {
    // return scale, offset
    ((max - min) / (2f64.powi(bits as i32) - 1.0), min)
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
            signal_type: CanSignalType::Numerical,
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
                signal_type: CanSignalType::Numerical,
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
                    -2f64.powi((bits - 1) as i32),
                    2f64.powi((bits - 1) as i32) - 1.0,
                ),
                Some(false) | None => (0.0, 2f64.powi(bits as i32) - 1.0),
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
                signal_type: CanSignalType::Numerical,
            }
        }
    }
}

fn parse_tx_msg_signals(
    json_signals: HashMap<String, JsonCanSignal>,
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

    // bombastic side eye
    if json_signals.len() > 0
        && json_signals.iter().any(|(_, s)| match s {
            JsonCanSignal::ScaleOffsetBits { start_bit, .. }
            | JsonCanSignal::BitsMinMax { start_bit, .. }
            | JsonCanSignal::ResolutionMinMax { start_bit, .. }
            | JsonCanSignal::Enum { start_bit, .. }
            | JsonCanSignal::Bits { start_bit, .. } => start_bit.is_some(),
        }) != json_signals.iter().all(|(_, s)| match s {
            JsonCanSignal::ScaleOffsetBits { start_bit, .. }
            | JsonCanSignal::BitsMinMax { start_bit, .. }
            | JsonCanSignal::ResolutionMinMax { start_bit, .. }
            | JsonCanSignal::Enum { start_bit, .. }
            | JsonCanSignal::Bits { start_bit, .. } => start_bit.is_some(),
        })
    {
        panic!(
            "In message '{}', either all signals must specify start bits, or none of them should.",
            msg_name
        );
    }

    // Parse message signals
    for (signal_name, signal_data) in json_signals {
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

fn parse_node_alert_signals(
    node_name: &String,
    alerts: &Vec<CanAlert>,
    alert_type: CanAlertType,
) -> (Vec<CanSignal>, Vec<CanSignal>) {
    // From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
    // :returns metadata which maps names of alert signals to their AlertsEntry, list of CanSignals for the alert message

    let mut signals: Vec<CanSignal> = Vec::new();
    let mut bit_pos: u16 = 0;

    for alert in alerts {
        let alert_type_name: &str = match alert_type {
            CanAlertType::Warning => "Warning",
            CanAlertType::Fault => "Fault",
            CanAlertType::Info => "Info",
        };

        signals.push(CanSignal {
            name: format!("{}_{}_{}", node_name, alert_type_name, alert.name),
            start_bit: bit_pos,
            bits: 1,
            scale: 1.0,
            offset: 0.0,
            min: 0.0,
            max: 1.0,
            start_val: 0.0,
            enum_name: None,
            unit: None,
            signed: false,
            description: None,
            big_endian: false,
            signal_type: CanSignalType::Alert,
        });

        bit_pos += 1;
    }

    (signals, vec![])
}

fn generate_node_alert_msgs(node_name: &String, alerts_json: &GroupedAlerts) -> [CanMessage; 6] {
    // Check alert messages ID are unique
    let warnings_name = format!("{}_Warnings", node_name);
    let faults_name = format!("{}_Faults", node_name);
    let warnings_counts_name = format!("{}_WarningsCounts", node_name);
    let faults_counts_name = format!("{}_FaultsCounts", node_name);
    let info_name = format!("{}_Info", node_name);
    let info_counts_name = format!("{}_InfoCounts", node_name);

    // Make alert signals
    let (warnings_signals, warnings_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.warnings, CanAlertType::Warning);
    let (faults_signals, faults_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.faults, CanAlertType::Fault);
    let (info_signals, info_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.infos, CanAlertType::Info);

    static WARNINGS_ALERTS_CYCLE_TIME: Option<u32> = Some(1000);
    static FAULTS_ALERTS_CYCLE_TIME: Option<u32> = Some(100);
    static INFO_ALERTS_CYCLE_TIME: Option<u32> = Some(100);

    [
        CanMessage {
            name: warnings_name,
            id: alerts_json.warnings_id,
            description: Some(format!("Status of warnings for the {}.", node_name)),
            cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            signals: warnings_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
        CanMessage {
            name: faults_name,
            id: alerts_json.faults_id,
            description: Some(format!("Status of faults for the {}.", node_name)),
            cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            log_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            telem_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            signals: faults_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
        CanMessage {
            name: warnings_counts_name,
            id: alerts_json.warnings_count_id,
            description: Some(format!(
                "Number of times warnings have been set for the {}.",
                node_name
            )),
            cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            signals: warnings_counts_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
        CanMessage {
            name: faults_counts_name,
            id: alerts_json.faults_count_id,
            description: Some(format!(
                "Number of times faults have been set for the {}.",
                node_name
            )),
            cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            log_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            telem_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            signals: faults_counts_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
        CanMessage {
            name: info_name,
            id: alerts_json.infos_id,
            description: Some(format!("Status of info for the {}.", node_name)),
            cycle_time: INFO_ALERTS_CYCLE_TIME,
            log_cycle_time: INFO_ALERTS_CYCLE_TIME,
            telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
            signals: info_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
        CanMessage {
            name: info_counts_name,
            id: alerts_json.infos_count_id,
            description: Some(format!(
                "Number of times info have been set for the {}.",
                node_name
            )),
            cycle_time: INFO_ALERTS_CYCLE_TIME,
            log_cycle_time: INFO_ALERTS_CYCLE_TIME,
            telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
            signals: info_counts_signals,
            tx_node_name: node_name.clone(),
            modes: vec![DEFAULT_BUS_MODE.to_string()],
        },
    ]
}

impl CanDatabase {
    pub fn from(parser: JsonCanParser) -> Result<Self, CanDBError> {
        // one can check that all fields of parser are used here
        let mut db = CanDatabase::new(parser.buses, parser.forwarding, parser.shared_enums)?;

        let mut rx_msg_names_to_resolve: Vec<(String, Vec<String>)> = Vec::new();

        // first handle adding nodes and their tx_msgs
        for n in parser.nodes {
            let tx_node_name = n.name.clone();
            let node = CanNode {
                rx_msgs_names: match n.rx_msgs {
                    JsonRxMsgNames::All => RxMsgs::All,
                    JsonRxMsgNames::RxMsgs(rx_msgs) => {
                        rx_msg_names_to_resolve.push((n.name.clone(), rx_msgs));
                        RxMsgs::RxMsgs(Vec::new())
                    }
                },
                name: n.name,
                collects_data: n.collects_data,
                alerts: n.alerts.map(|a| GroupedAlerts {
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
            };
            db.add_node(node);

            // find the node in the list of nodes
            for msg in n.tx_msgs {
                let signals = parse_tx_msg_signals(
                    msg.signals,
                    &db.shared_enums,
                    &db.nodes.iter().find(|n| n.name == tx_node_name).unwrap().enums,
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
                    modes: msg.modes,
                    signals,
                })?;
            }
        }

        // resolve explicit rx messages
        for (rx_name, rx_msg_names) in rx_msg_names_to_resolve.into_iter() {
            let rx_msgs: Vec<u32> = rx_msg_names
                .iter()
                .map(|m| db.get_message_id_by_name(m).expect(&format!("Message {} not found in database", m)))
                .collect();
            if let RxMsgs::RxMsgs(old_rx_msgs) = &mut db.nodes.iter_mut().find(|n| n.name == rx_name).expect(&format!("Node {} not found in database", rx_name)).rx_msgs_names {
                *old_rx_msgs = rx_msgs;
            } else {
                panic!("Node {} was marked as receiving all messages, but is also trying to explicitly receive some messages. Please fix the JSON input.", rx_name);
            }
        }

        // alert tx msgs
        let alert_tx_msgs: Vec<CanMessage> = db.nodes
            .iter()
            .filter_map(|n| n.alerts.as_ref().map(|alerts| generate_node_alert_msgs(&n.name, alerts)))
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
                        assert!(!rx_msgs.contains(&id), "Node {} is trying to receive ALERT message ID {}, but that ID is already in its rx_msgs list. Note that all alerts are already RXd by default", n.name, id);
                        rx_msgs.push(id);
                    }
                }
            });
        }

        Ok(db)
    }
}
