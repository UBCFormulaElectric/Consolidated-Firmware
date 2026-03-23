use crate::{
    can_database::{BusForwarder, CanBus, CanDatabase, CanEnum, CanSignalType, error::CanDBError},
    parsing::{JsonCanParser, JsonNode, JsonRxMsgNames},
};
use std::{
    collections::{HashMap, HashSet},
    vec,
};

#[test]
fn test_signal_type_mapping() {
    assert_eq!(
        CanSignalType::Numerical,
        CanSignalType::from(CanSignalType::Numerical as u32)
    );
    assert_eq!(
        CanSignalType::Enum,
        CanSignalType::from(CanSignalType::Enum as u32)
    );
    assert_eq!(
        CanSignalType::Alert,
        CanSignalType::from(CanSignalType::Alert as u32)
    );
    assert_eq!(
        CanSignalType::Boolean,
        CanSignalType::from(CanSignalType::Boolean as u32)
    );
}

#[test]
fn test_null() {
    let p = JsonCanParser {
        nodes: vec![],
        buses: vec![],
        shared_enums: Vec::new(),
        forwarding: Vec::new(),
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_ok());
}

#[test]
fn test_valid_node_on_valid_bus() {
    let p = JsonCanParser {
        nodes: vec![JsonNode {
            name: "Node1".to_string(),
            collects_data: true,
            enums: Vec::new(),
            alerts: None,
            tx_msgs: Vec::new(),
            rx_msgs: JsonRxMsgNames::RxMsgs(Vec::new()),
        }],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec!["Node1".to_string()],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "ABC".into(),
            fd: false,
        }],
        shared_enums: Vec::new(),
        forwarding: Vec::new(),
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_ok());
}

#[test]
fn test_invalid_node_on_valid_bus() {
    let p = JsonCanParser {
        nodes: vec![],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec!["Node1".to_string()],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "ABC".into(),
            fd: false,
        }],
        shared_enums: vec![],
        forwarding: vec![],
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::BusReferencesUndefinedNode {
            bus_name,
            node_name,
        } if bus_name == "Bus1" && node_name == "Node1"
    ));
}

#[test]
fn test_bus_default_mode_invalid() {
    let p = JsonCanParser {
        nodes: vec![],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec![],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "XYZ".into(),
            fd: false,
        }],
        shared_enums: vec![],
        forwarding: vec![],
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::BusDefaultModeNotInModes {
            bus_name,
            default_mode,
        } if bus_name == "Bus1" && default_mode == "XYZ"
    ));
}

#[test]
fn test_bus_forwarding_has_invalid_bus() {
    let p = JsonCanParser {
        nodes: vec![JsonNode {
            name: "Forwarder1".into(),
            collects_data: false,
            enums: vec![],
            alerts: None,
            tx_msgs: vec![],
            rx_msgs: JsonRxMsgNames::All,
        }],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec!["Forwarder1".into()],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "ABC".into(),
            fd: false,
        }],
        shared_enums: vec![],
        forwarding: vec![BusForwarder {
            bus1_name: "Bus1".into(),
            bus2_name: "Bus2".into(),
            forwarder_name: "Forwarder1".into(),
        }],
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::ForwarderReferencesUndefinedBus {
            forwarder_name,
            bus_name
        } if forwarder_name == "Forwarder1" && bus_name == "Bus2"
    ));

    let p = JsonCanParser {
        nodes: vec![JsonNode {
            name: "Forwarder1".into(),
            collects_data: false,
            enums: vec![],
            alerts: None,
            tx_msgs: vec![],
            rx_msgs: JsonRxMsgNames::All,
        }],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec![],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "ABC".into(),
            fd: false,
        }],
        shared_enums: vec![],
        forwarding: vec![BusForwarder {
            bus1_name: "Bus2".into(),
            bus2_name: "Bus1".into(),
            forwarder_name: "Forwarder1".into(),
        }],
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::ForwarderReferencesUndefinedBus {
            forwarder_name,
            bus_name
        } if forwarder_name == "Forwarder1" && bus_name == "Bus2"
    ));
}

#[test]
fn test_bus_forwarding_has_same_bus() {
    let p = JsonCanParser {
        nodes: vec![JsonNode {
            name: "Forwarder1".into(),
            collects_data: false,
            enums: vec![],
            alerts: None,
            tx_msgs: vec![],
            rx_msgs: JsonRxMsgNames::All,
        }],
        buses: vec![CanBus {
            name: "Bus1".to_string(),
            node_names: vec!["Forwarder1".into()],
            bus_speed: 500,
            modes: vec!["ABC".into()],
            default_mode: "ABC".into(),
            fd: false,
        }],
        shared_enums: vec![],
        forwarding: vec![BusForwarder {
            bus1_name: "Bus1".into(),
            bus2_name: "Bus1".into(),
            forwarder_name: "Forwarder1".into(),
        }],
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::ForwarderReferencesSameBus {
            forwarder_name,
            bus_name
        } if forwarder_name == "Forwarder1" && bus_name == "Bus1"
    ));
}

#[test]
fn test_buses_consistent() {
    // TODO there are more tests to ensure that bus fields are valid

    let p = JsonCanParser {
        nodes: vec![],
        buses: vec![],
        shared_enums: Vec::new(),
        forwarding: Vec::new(),
    };
    let cdb = CanDatabase::from(p).unwrap();

    let bus_count = cdb.buses.len();

    let unique_bus_names = cdb
        .buses
        .iter()
        .map(|b| b.name.clone())
        .collect::<HashSet<String>>()
        .len();
    assert!(
        bus_count == unique_bus_names,
        "There are {} busses but {} unique bus names",
        bus_count,
        unique_bus_names
    );

    let node_names: Vec<String> = cdb.nodes.iter().map(|n| n.name.clone()).collect();
    for bus in cdb.buses {
        for node_name in bus.node_names {
            assert!(node_names.contains(&node_name));
        }
    }
}

#[test]
fn test_bus_name_collision() {
    let p = JsonCanParser {
        nodes: vec![],
        buses: vec![
            CanBus {
                name: "Bus1".to_string(),
                node_names: vec![],
                bus_speed: 500,
                modes: vec!["ABC".into()],
                default_mode: "ABC".into(),
                fd: false,
            },
            CanBus {
                name: "Bus1".to_string(),
                node_names: vec![],
                bus_speed: 500,
                modes: vec!["ABC".into()],
                default_mode: "ABC".into(),
                fd: false,
            },
        ],
        shared_enums: Vec::new(),
        forwarding: Vec::new(),
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::BusNameCollision { bus_name } if bus_name == "Bus1"
    ));
}

#[test]
fn test_enum_name_collision() {
    let p = JsonCanParser {
        nodes: vec![
            JsonNode {
                name: "Node1".to_string(),
                collects_data: true,
                enums: vec![CanEnum {
                    name: "Enum1".into(),
                    values: HashMap::new(),
                }],
                alerts: None,
                tx_msgs: Vec::new(),
                rx_msgs: JsonRxMsgNames::RxMsgs(Vec::new()),
            },
            JsonNode {
                name: "Node2".to_string(),
                collects_data: true,
                enums: vec![CanEnum {
                    name: "Enum1".into(),
                    values: HashMap::new(),
                }],
                alerts: None,
                tx_msgs: Vec::new(),
                rx_msgs: JsonRxMsgNames::RxMsgs(Vec::new()),
            },
        ],
        buses: vec![],
        shared_enums: Vec::new(),
        forwarding: Vec::new(),
    };
    let cdb = CanDatabase::from(p);
    assert!(cdb.is_err());
    assert!(matches!(
        cdb.err().unwrap(),
        CanDBError::EnumMultipleDefinitions { enum_name, definition_locs }
        if enum_name == "Enum1" && definition_locs.contains(&"Node1".into()) && definition_locs.contains(&"Node2".into())
    ));
}
