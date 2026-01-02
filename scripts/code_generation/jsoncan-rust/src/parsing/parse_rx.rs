use crate::can_database::RxMsgNames;
use serde::Deserialize;

#[derive(Deserialize)]
#[serde(untagged)]
enum JsonRxEntry {
    All(String),
    RxMsgs(Vec<String>),
}

#[derive(Deserialize)]
struct JsonRxData {
    messages: JsonRxEntry,
}

pub fn parse_json_rx_data(can_data_dir: &str, rx_node_name: &str) -> RxMsgNames {
    let file_path = format!("{}/{}/{}_rx.json", can_data_dir, rx_node_name, rx_node_name);
    let file_content = std::fs::read_to_string(file_path).expect(&format!(
        "Failed to read TX JSON file for node {}",
        rx_node_name
    ));

    let json_rx_msgs: JsonRxData = match serde_json::from_str(&file_content) {
        Ok(data) => data,
        Err(e) => panic!(
            "Failed to parse RX JSON file for node {}: {}",
            rx_node_name, e
        ),
    };

    match json_rx_msgs.messages {
        JsonRxEntry::All(s) => {
            assert!(
                s.to_uppercase() == "ALL",
                "Expected 'ALL' string for all messages"
            );
            RxMsgNames::All
        }
        JsonRxEntry::RxMsgs(msg_list) => RxMsgNames::RxMsgs(msg_list.into_iter().collect()),
    }
}
