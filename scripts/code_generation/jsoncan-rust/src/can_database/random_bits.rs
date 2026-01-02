// THIS IS FOR REGISTERING ALERT MESSAGES
// let alert_boards = nodes_alert_msgs
//     .keys()
//     .cloned()
//     .collect::<HashSet<String>>();
// for (tx_node_name, alert_msgs) in nodes_alert_msgs {
//     for alert_msg in alert_msgs {
//         let alert_msg_name = alert_msg.name.clone();
//         // register the message with the database of all messages
//         add_tx_msg(alert_msg, &tx_node_name, &mut msgs)?;
//         let alert_msg: &CanMessage = msgs
//             .get(&alert_msg_name)
//             .expect("literally look at the above line");
//         for other_rx_node_name in &alert_boards {
//             if tx_node_name == *other_rx_node_name {
//                 continue;
//             }
//             nodes
//                 .get_mut(other_rx_node_name)
//                 .expect("Node should exist??")
//                 .add_rx_msg(alert_msg)?;
//         }
//     }
// }

fn parse_node_alert_signals(
    node_name: &String,
    alerts: HashMap<String, AlertMetaData>,
    alert_type: CanAlertType,
) -> (
    HashMap<String, AlertMetaData>,
    Vec<CanSignal>,
    Vec<CanSignal>,
) {
    // From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
    // :returns metadata which maps names of alert signals to their AlertsEntry, list of CanSignals for the alert message

    let mut signals: Vec<CanSignal> = Vec::new();
    let mut meta_data: HashMap<String, AlertMetaData> = HashMap::new();
    let mut bit_pos: u16 = 0;

    for (alerts_name, alerts_entry) in alerts {
        let alert_type_name = match alert_type {
            CanAlertType::Warning => "Warning",
            CanAlertType::Fault => "Fault",
            CanAlertType::Info => "Info",
        };

        signals.push(CanSignal {
            name: format!("{}_{}_{}", node_name, alert_type_name, alerts_name),
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
        meta_data.insert(
            format!("{}_{}_{}", node_name, alert_type_name, alerts_name),
            alerts_entry,
        );
    }

    return (meta_data, signals, vec![]);
}

fn parse_node_alerts(
    node_name: &String,
    alerts_json: AlertsJson,
) -> (
    (
        CanMessage,
        CanMessage,
        CanMessage,
        CanMessage,
        CanMessage,
        CanMessage,
    ),
    (
        HashMap<String, AlertMetaData>,
        HashMap<String, AlertMetaData>,
        HashMap<String, AlertMetaData>,
    ),
) {
    // Check alert messages ID are unique
    let warnings_name = format!("{}_Warnings", node_name);
    let faults_name = format!("{}_Faults", node_name);
    let warnings_counts_name = format!("{}_WarningsCounts", node_name);
    let faults_counts_name = format!("{}_FaultsCounts", node_name);
    let info_name = format!("{}_Info", node_name);
    let info_counts_name = format!("{}_InfoCounts", node_name);

    // Make alert signals
    let (warnings_meta_data, warnings_signals, warnings_counts_signals) =
        parse_node_alert_signals(node_name, warnings, CanAlertType::Warning);
    let (faults_meta_data, faults_signals, faults_counts_signals) =
        parse_node_alert_signals(node_name, faults, CanAlertType::Fault);
    let (info_meta_data, info_signals, info_counts_signals) =
        parse_node_alert_signals(node_name, info, CanAlertType::Info);

    static WARNINGS_ALERTS_CYCLE_TIME: Option<u32> = Some(1000);
    static FAULTS_ALERTS_CYCLE_TIME: Option<u32> = Some(100);
    static INFO_ALERTS_CYCLE_TIME: Option<u32> = Some(100);

    (
        (
            CanMessage {
                name: warnings_name,
                id: alerts_json.warnings_id,
                description: Some(format!("Status of warnings for the {}.", node_name)),
                cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                signals: warnings_signals,
                tx_node_name: node_name.clone(),
                modes: None,
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
                modes: None,
            },
            CanMessage {
                name: warnings_counts_name,
                id: alerts_json.warnings_counts_id,
                description: Some(format!(
                    "Number of times warnings have been set for the {}.",
                    node_name
                )),
                cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
                signals: warnings_counts_signals,
                tx_node_name: node_name.clone(),
                modes: None,
            },
            CanMessage {
                name: faults_counts_name,
                id: alerts_json.faults_counts_id,
                description: Some(format!(
                    "Number of times faults have been set for the {}.",
                    node_name
                )),
                cycle_time: FAULTS_ALERTS_CYCLE_TIME,
                log_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
                telem_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
                signals: faults_counts_signals,
                tx_node_name: node_name.clone(),
                modes: None,
            },
            CanMessage {
                name: info_name,
                id: alerts_json.info_id,
                description: Some(format!("Status of info for the {}.", node_name)),
                cycle_time: INFO_ALERTS_CYCLE_TIME,
                log_cycle_time: INFO_ALERTS_CYCLE_TIME,
                telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
                signals: info_signals,
                tx_node_name: node_name.clone(),
                modes: None,
            },
            CanMessage {
                name: info_counts_name,
                id: alerts_json.info_counts_id,
                description: Some(format!(
                    "Number of times info have been set for the {}.",
                    node_name
                )),
                cycle_time: INFO_ALERTS_CYCLE_TIME,
                log_cycle_time: INFO_ALERTS_CYCLE_TIME,
                telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
                signals: info_counts_signals,
                tx_node_name: node_name.clone(),
                modes: None,
            },
        ),
        (faults_meta_data, warnings_meta_data, info_meta_data),
    )
}
