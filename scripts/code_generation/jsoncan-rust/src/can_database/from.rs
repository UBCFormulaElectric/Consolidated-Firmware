use std::collections::HashMap;

use crate::{
    can_database::{
        CanDatabase, CanEnum, CanMessage, CanNode, CanSignal, CanSignalType, error::CanDBError,
    },
    parsing::{JsonCanParser, JsonTxSignal},
};

fn calculate_scale_offset(min: f64, max: f64, bits: u16) -> (f64, f64) {
    // return scale, offset
    ((max - min) / (2f64.powi(bits as i32) - 1.0), min)
}

fn parse_signal(
    signal_name: String,
    signal: JsonTxSignal,
    next_available_bit: u16,
    node_enums: &Vec<CanEnum>,
    shared_enums: &Vec<CanEnum>,
) -> CanSignal {
    // TODO description
    match signal {
        JsonTxSignal::ScaleOffsetBits {
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
        JsonTxSignal::BitsMinMax {
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
        JsonTxSignal::ResolutionMinMax {
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
        JsonTxSignal::Enum {
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
        JsonTxSignal::Bits {
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
                    -(2f64.powi((bits - 1) as i32)),
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
    json_signals: HashMap<String, JsonTxSignal>,
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
    if json_signals.iter().any(|(_, s)| match s {
        JsonTxSignal::ScaleOffsetBits { start_bit, .. }
        | JsonTxSignal::BitsMinMax { start_bit, .. }
        | JsonTxSignal::ResolutionMinMax { start_bit, .. }
        | JsonTxSignal::Enum { start_bit, .. }
        | JsonTxSignal::Bits { start_bit, .. } => start_bit.is_some(),
    }) != json_signals.iter().all(|(_, s)| match s {
        JsonTxSignal::ScaleOffsetBits { start_bit, .. }
        | JsonTxSignal::BitsMinMax { start_bit, .. }
        | JsonTxSignal::ResolutionMinMax { start_bit, .. }
        | JsonTxSignal::Enum { start_bit, .. }
        | JsonTxSignal::Bits { start_bit, .. } => start_bit.is_some(),
    }) {
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

    return signals;
}

impl CanDatabase {
    pub fn from(parser: JsonCanParser) -> Result<Self, CanDBError> {
        let can_nodes = parser
            .nodes
            .iter()
            .map(|n| CanNode {
                name: n.name.clone(),
                rx_msgs_names: n.rx_msgs.clone(),
                collects_data: n.collects_data,
            })
            .collect();

        let db = CanDatabase::new(parser.buses, can_nodes, parser.forwarding);

        // resolve all tx messages
        for node in parser.nodes {
            // register tx_msgs in database
            for msg in node.tx_msgs {
                let signals: Vec<CanSignal> = parse_tx_msg_signals(
                    msg.signals,
                    &node.enums,
                    &parser.shared_enums,
                    &msg.name,
                    &node.name,
                );
                db.add_tx_msg(CanMessage {
                    name: msg.name,
                    id: msg.id,
                    description: msg.description,
                    cycle_time: msg.cycle_time,
                    log_cycle_time: msg.log_cycle_time,
                    telem_cycle_time: msg.telem_cycle_time,
                    tx_node_name: node.name.clone(),
                    modes: msg.modes,
                    signals,
                });
            }
            // node.alerts
            // node.
        }

        Ok(db)
    }
}
