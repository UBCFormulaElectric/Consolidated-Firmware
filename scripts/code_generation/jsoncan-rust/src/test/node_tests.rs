// These tests check that nodes are created correctly.

use crate::test::{assert_setequal, setup};

static ECU1_MSGS: [&str; 6] = [
    "ECU1_BasicSignalTypes",
    "ECU1_DecimalNumbers",
    "ECU1_DbcMatching",
    "ECU1_MinMaxClamping",
    "ECU1_Debug_Only",
    "ECU1_LongMessage",
];
static ECU1_ALERTS: [&str; 6] = [
    "ECU1_Warnings",
    "ECU1_WarningsCounts",
    "ECU1_Faults",
    "ECU1_FaultsCounts",
    "ECU1_Info",
    "ECU1_InfoCounts",
];
// static ECU2_MSGS: [&str; 1] = ["ECU2_BasicSignalTypes"];
static ECU3_MSGS: [&str; 1] = ["ECU3_TEST"];
static ECU3_ALERTS: [&str; 6] = [
    "ECU3_Warnings",
    "ECU3_WarningsCounts",
    "ECU3_Faults",
    "ECU3_FaultsCounts",
    "ECU3_Info",
    "ECU3_InfoCounts",
];
static ECU4_MSGS: [&str; 1] = ["ECU4_TEST"];
static ECU4_ALERTS: [&str; 6] = [
    "ECU4_Warnings",
    "ECU4_WarningsCounts",
    "ECU4_Faults",
    "ECU4_FaultsCounts",
    "ECU4_Info",
    "ECU4_InfoCounts",
];
static ECU5_MSGS: [&str; 1] = ["ECU5_TEST"];
static ECU5_ALERTS: [&str; 6] = [
    "ECU5_Warnings",
    "ECU5_WarningsCounts",
    "ECU5_Faults",
    "ECU5_FaultsCounts",
    "ECU5_Info",
    "ECU5_InfoCounts",
];

#[test]
fn test_all_nodes_present() {
    // Checks that ECU1-5 are the only nodes present in the database.
    let cdb = setup().unwrap();
    assert_setequal(
        cdb.nodes.iter().map(|node| node.name.clone()),
        [
            "ECU1".to_string(),
            "ECU2".to_string(),
            "ECU3".to_string(),
            "ECU4".to_string(),
            "ECU5".to_string(),
        ]
        .into_iter(),
    );
}

// #[test]
// fn test_node_on_busses() {
//     let cdb = setup().unwrap();
//     assert_setequal(
//         cdb.nodes
//             .iter()
//             .find(|n| n.name == "ECU1")
//             .unwrap()
//             .bus_names
//             .iter(),
//         ["can1".to_string(), "can3".to_string()].iter(),
//     );
//     assert_setequal(
//         cdb.nodes.get("ECU2").unwrap().bus_names.iter(),
//         ["can3".to_string()].iter(),
//     );
//     assert_setequal(
//         cdb.nodes.get("ECU3").unwrap().bus_names.iter(),
//         ["can1".to_string(), "can2".to_string()].iter(),
//     );
//     assert_setequal(
//         cdb.nodes.get("ECU4").unwrap().bus_names.iter(),
//         ["can1".to_string()].iter(),
//     );
//     assert_setequal(
//         cdb.nodes.get("ECU5").unwrap().bus_names.iter(),
//         ["can2".to_string()].iter(),
//     );
// }

#[test]
fn test_all_tx_messages_present() {
    // Checks that all tx/rx messages are present
    let cdb = setup().unwrap();

    assert_setequal(
        cdb.get_message_by_node("ECU1")
            .unwrap()
            .into_iter()
            .map(|s| s.name),
        ECU1_MSGS
            .iter()
            .chain(ECU1_ALERTS.iter())
            .map(|s| s.to_string()),
    );
    assert_setequal(
        cdb.get_message_by_node("ECU3")
            .unwrap()
            .into_iter()
            .map(|s| s.name),
        ECU3_MSGS
            .iter()
            .chain(ECU3_ALERTS.iter())
            .map(|s| s.to_string()),
    );
    assert_setequal(
        cdb.get_message_by_node("ECU4")
            .unwrap()
            .into_iter()
            .map(|s| s.name),
        ECU4_MSGS
            .iter()
            .chain(ECU4_ALERTS.iter())
            .map(|s| s.to_string()),
    );
    assert_setequal(
        cdb.get_message_by_node("ECU5")
            .unwrap()
            .into_iter()
            .map(|s| s.name),
        ECU5_MSGS
            .iter()
            .chain(ECU5_ALERTS.iter())
            .map(|s| s.to_string()),
    );
}
