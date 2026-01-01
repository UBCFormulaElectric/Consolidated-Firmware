use crate::can_database::{CanEnum, CanMessage, CanSignal, CanSignalType};
use serde::Deserialize;
use std::collections::HashMap;

#[derive(Deserialize)]
#[serde(untagged)]
enum JsonTxSignal {
    ScaleOffsetBits {
        min: f64,
        max: f64,
        scale: f64,
        offset: f64,
        bits: u16,
        // SHARED
        #[serde(default)]
        unit: Option<String>,
        #[serde(default)]
        start_bit: Option<u16>,
        #[serde(default)]
        signed: Option<bool>,
        #[serde(default)]
        big_endian: Option<bool>,
        #[serde(default)]
        start_value: Option<f64>,
    },
    BitsMinMax {
        bits: u16,
        min: f64,
        max: f64,
        // SHARED
        #[serde(default)]
        unit: Option<String>,
        #[serde(default)]
        start_bit: Option<u16>,
        #[serde(default)]
        signed: Option<bool>,
        #[serde(default)]
        big_endian: Option<bool>,
        #[serde(default)]
        start_value: Option<f64>,
    },
    ResolutionMinMax {
        resolution: f64,
        min: f64,
        max: f64,
        // SHARED
        #[serde(default)]
        unit: Option<String>,
        #[serde(default)]
        start_bit: Option<u16>,
        #[serde(default)]
        signed: Option<bool>,
        #[serde(default)]
        big_endian: Option<bool>,
        #[serde(default)]
        start_value: Option<f64>,
    },
    Enum {
        #[serde(rename = "enum")]
        enum_name: String,
        #[serde(default)]
        start_value: Option<f64>,
        #[serde(default)]
        start_bit: Option<u16>,
    },
    Bits {
        bits: u16,
        #[serde(default)]
        scale: Option<f64>,
        // SHARED
        #[serde(default)]
        unit: Option<String>,
        #[serde(default)]
        start_bit: Option<u16>,
        #[serde(default)]
        signed: Option<bool>,
        #[serde(default)]
        big_endian: Option<bool>,
        #[serde(default)]
        start_value: Option<f64>,
    },
}

#[derive(Deserialize)]
struct JsonDataCapture {
    log_cycle_time: Option<u32>,
    telem_cycle_time: Option<u32>,
}

// convert above to structs in serde
#[derive(Deserialize)]
struct JsonTxMsg {
    msg_id: u32, // todo extra validity checks
    signals: HashMap<String, JsonTxSignal>,
    cycle_time: Option<u32>, // todo extra validity checks
    disabled: Option<bool>,
    description: Option<String>,
    allowed_modes: Option<Vec<String>>,
    data_capture: Option<JsonDataCapture>,
}

fn calculate_scale_offset(min: f64, max: f64, bits: u16) -> (f64, f64) {
    // return scale, offset
    ((max - min) / (2f64.powi(bits as i32) - 1.0), min)
}

fn parse_signal(
    // signals: &HashMap<String, JsonTxSignal>,
    signal_name: String,
    signal: JsonTxSignal,
    node_enums: &HashMap<String, CanEnum>,
    shared_enums: &HashMap<String, CanEnum>,
    next_available_bit: u16,
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
            let signal_enum = node_enums
                .get(&enum_name)
                .or_else(|| shared_enums.get(&enum_name))
                .expect(&format!(
                    "Signal '{}' references enum '{}' which does not exist.",
                    signal_name, enum_name
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

fn parse_tx_msg(
    msg_name: String,
    msg: JsonTxMsg,
    tx_node_name: &String,
    node_enums: &HashMap<String, CanEnum>,
    shared_enums: &HashMap<String, CanEnum>,
) -> CanMessage {
    static MAX_LEN_BITS: usize = 64 * 8; // 64 bytes

    // Placeholder function to convert JsonTxSignal to CanSignal
    let mut signals: Vec<CanSignal> = Vec::new();
    let mut next_available_bit: u16 = 0;
    let mut occupied_bits: Vec<Option<String>> = vec![None; MAX_LEN_BITS];

    // bombastic side eye
    if msg.signals.iter().any(|(_, s)| match s {
        JsonTxSignal::ScaleOffsetBits { start_bit, .. }
        | JsonTxSignal::BitsMinMax { start_bit, .. }
        | JsonTxSignal::ResolutionMinMax { start_bit, .. }
        | JsonTxSignal::Enum { start_bit, .. }
        | JsonTxSignal::Bits { start_bit, .. } => start_bit.is_some(),
    }) != msg.signals.iter().all(|(_, s)| match s {
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
    for (signal_name, signal_data) in msg.signals {
        let signal = parse_signal(
            format!("{}_{}", tx_node_name, signal_name),
            signal_data,
            node_enums,
            shared_enums,
            next_available_bit,
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

    CanMessage {
        name: msg_name.clone(),
        id: msg.msg_id,
        signals,
        cycle_time: msg.cycle_time,
        description: msg.description.clone(),
        log_cycle_time: match &msg.data_capture {
            Some(data_capture) => data_capture.log_cycle_time,
            None => None,
        },
        telem_cycle_time: match &msg.data_capture {
            Some(data_capture) => data_capture.telem_cycle_time,
            None => None,
        },
        tx_node_name: tx_node_name.clone(), // to be filled in later
        modes: msg.allowed_modes.clone(),
    }
}

pub fn parse_tx_data(
    can_data_dir: &String,
    tx_node_name: &String,
    node_enums: &HashMap<String, CanEnum>,
    shared_enums: &HashMap<String, CanEnum>,
) -> Vec<CanMessage> {
    // load json file
    let file_path = format!("{}/{}/{}_tx.json", can_data_dir, tx_node_name, tx_node_name);
    let file_content = std::fs::read_to_string(file_path).expect(&format!(
        "Failed to read TX JSON file for node {}",
        tx_node_name
    ));

    let json_tx_msgs: HashMap<String, JsonTxMsg> = match serde_json::from_str(&file_content) {
        Ok(data) => data,
        Err(e) => panic!(
            "Failed to parse TX JSON file for node {}: {}",
            tx_node_name, e
        ),
    }; // maps message name to message data

    // convert to CanMessage structs
    let can_messages: Vec<CanMessage> = json_tx_msgs
        .into_iter()
        .filter(|(_, msg)| !msg.disabled.unwrap_or(false))
        .map(|(name, msg)| {
            parse_tx_msg(
                format!("{}_{}", tx_node_name, name),
                msg,
                tx_node_name,
                &node_enums,
                &shared_enums,
            )
        })
        .collect();
    can_messages
}
