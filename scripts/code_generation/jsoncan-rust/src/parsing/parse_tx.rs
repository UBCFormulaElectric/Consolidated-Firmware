use serde::Deserialize;
use std::collections::HashMap;

use crate::parsing::DEFAULT_BUS_MODE;

#[derive(Deserialize)]
#[serde(untagged)]
pub enum JsonTxSignal {
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

pub struct JsonCanMessage {
    pub name: String,
    pub id: u32,
    pub signals: HashMap<String, JsonTxSignal>,
    pub cycle_time: Option<u32>,
    pub description: Option<String>,
    pub log_cycle_time: Option<u32>,
    pub telem_cycle_time: Option<u32>,
    pub modes: Vec<String>,
}

pub fn parse_tx_data(can_data_dir: &String, tx_node_name: &String) -> Vec<JsonCanMessage> {
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

    json_tx_msgs
        .into_iter()
        .filter(|(_, msg)| !msg.disabled.unwrap_or(false))
        .map(|(msg_name, msg)| {
            if let Some(cycle_time) = msg.cycle_time && cycle_time == 0 {
                panic!(
                    "Message '{}' in node '{}' has an invalid cycle_time of 0. Cycle time must be greater than 0 if specified. Specify cycle time as null (or do not specify it) if you want async messages.",
                    msg_name, tx_node_name
                );
            }
            JsonCanMessage {
                name: msg_name.clone(),
                id: msg.msg_id,
                signals: msg.signals,
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
                modes: match &msg.allowed_modes {
                    Some(modes) => modes.clone(),
                    None => vec![DEFAULT_BUS_MODE.to_string()] // TODO revisit default behaviour?
                },
            }
        })
        .collect()
}
