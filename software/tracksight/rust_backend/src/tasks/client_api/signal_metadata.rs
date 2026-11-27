use std::collections::HashMap;

use jsoncan_rust::can_database::{CanDatabase, CanMessage, CanSignalType};
use regex::Regex;
use serde::{Serialize};

#[derive(Debug, Serialize)]
pub struct SignalMetadata {
    pub name: String,
    pub min_val: f64,
    pub max_val: f64,
    pub unit: Option<String>,
    pub enum_signal: Option<SignalMetadataEnumSignal>,
    pub tx_node: String,
    pub cycle_time_ms: Option<u32>,
    pub id: u32,
    pub msg_name: String,
    #[serde(skip)]
    pub signal_type: CanSignalType,
}

#[derive(Debug, Serialize)]
pub struct SignalMetadataEnumSignal {
    pub enum_name: String,
    pub enum_values: HashMap<String, u32>,
}

pub fn get_all_signal_metadatas(can_db: &CanDatabase, filter: Option<Regex>) -> HashMap<String, SignalMetadata> {
    let flat_map = | msg: &CanMessage | {
        return msg.signals.iter().map(
            | signal | {
                let can_enum: Option<SignalMetadataEnumSignal> = 
                    if let Some(enum_name) = &signal.enum_name && 
                    let Some(can_enum) = 
                    can_db.get_enum(enum_name) {
                        Some(SignalMetadataEnumSignal {
                            enum_name: can_enum.name.clone(),
                            enum_values: can_enum.values.clone()
                        })
                    } else {
                        None
                    };
                (signal.name.clone(),
                SignalMetadata {
                    name: signal.name.clone(),
                    min_val: signal.min,
                    max_val: signal.max,
                    unit: signal.unit.clone(),
                    enum_signal: can_enum,
                    tx_node: msg.tx_node_name.clone(),
                    cycle_time_ms: msg.cycle_time.clone(),
                    id: msg.id,
                    msg_name: msg.name.clone(),
                    signal_type: signal.signal_type.clone(),
                })
            }
        ).collect::<Vec<_>>()
    };

    can_db.get_all_msgs()
        .unwrap_or_default()
        .iter()
        .filter(|msg| filter.is_none() || filter.as_ref().unwrap().is_match(&msg.name))
        .flat_map(flat_map)
        .collect()
}