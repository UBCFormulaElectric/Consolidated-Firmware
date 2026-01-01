use crate::can_database::CanEnum;
use std::collections::HashMap;

fn parse_enum(enum_data_path: String) -> HashMap<String, CanEnum> {
    // load json file
    let data = std::fs::read_to_string(&enum_data_path).expect(&format!(
        "Failed to read '{}'. Ensure it exists and is readable.",
        enum_data_path
    ));

    // parse json
    let enums_json: HashMap<String, HashMap<String, u32>> = match serde_json::from_str(&data) {
        Ok(value) => value,
        Err(e) => panic!("Failed to parse shared_enum.json: {}", e),
    };

    let mut enums: HashMap<String, CanEnum> = HashMap::new();
    for (enum_name, values_map) in enums_json {
        let can_enum = CanEnum {
            name: enum_name.clone(),
            values: values_map,
        };
        enums.insert(enum_name, can_enum);
    }

    enums
}

pub fn parse_shared_enums(can_data_dir: &str) -> HashMap<String, CanEnum> {
    parse_enum(format!("{}/shared_enum.json", can_data_dir))
}

pub fn parse_node_enum_data(can_data_dir: &String, node_name: &String) -> HashMap<String, CanEnum> {
    parse_enum(format!(
        "{}/{}/{}_enum.json",
        can_data_dir, node_name, node_name
    ))
}
