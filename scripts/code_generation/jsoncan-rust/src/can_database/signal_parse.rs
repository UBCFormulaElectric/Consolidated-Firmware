use crate::{
    can_database::{CanEnum, CanMessage, CanSignal, CanSignalType},
    parsing::JsonCanSignal,
};
use indexmap::IndexMap;

fn calculate_scale_offset(min: f64, max: f64, bits: u16) -> (f64, f64) {
    // return scale, offset
    // ((max - min) / (2f64.powi(bits as i32) - 1.0), min)
    ((max - min) / (1u128 << bits - 1) as f64, min)
}

fn parse_signal<'a>(
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

pub fn parse_tx_msg_signals<'a>(
    json_signals: IndexMap<String, JsonCanSignal>,
    node_enums: &Vec<CanEnum>,
    shared_enums: &Vec<CanEnum>,
    // also useful
    parent_msg: &'a CanMessage,
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
                parent_msg.name
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
                    signal.name, parent_msg.name, idx
                );
            } else if occupied_bits[idx].is_some() {
                panic!(
                    "Signal '{}' in '{}' is requesting to put a bit at invalid position {}. That position is already occupied by the signal '{}'.",
                    signal.name,
                    parent_msg.name,
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
