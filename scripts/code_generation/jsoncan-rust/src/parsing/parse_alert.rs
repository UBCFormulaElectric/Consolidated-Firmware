use crate::can_database::{CanAlert, CanAlertType, CanMessage, CanSignal, CanSignalType};
use std::{collections::HashMap, vec};

#[derive(serde::Deserialize)]
struct AlertMetaData {
    id: u32,
    description: String,
    #[serde(default)]
    disabled: bool,
}

#[derive(serde::Deserialize)]
struct AlertsJson {
    warnings_id: u32,
    warnings_counts_id: u32,
    faults_id: u32,
    faults_counts_id: u32,
    info_id: u32,
    info_counts_id: u32,
    warnings: std::collections::HashMap<String, AlertMetaData>,
    faults: std::collections::HashMap<String, AlertMetaData>,
    info: std::collections::HashMap<String, AlertMetaData>,
}

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
    let mut bit_pos: u8 = 0;

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
    // Parse JSON data dictionary representing a node's alerts.
    let warnings: HashMap<String, AlertMetaData> = alerts_json
        .warnings
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .collect::<HashMap<String, AlertMetaData>>();
    let faults = alerts_json
        .faults
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .collect::<HashMap<String, AlertMetaData>>();
    let info = alerts_json
        .info
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .collect::<HashMap<String, AlertMetaData>>();

    // Number of alerts can't exceed 21. This is because we transmit a "counts" message for faults and warnings
    // that indicate the number of times an alert has been set. Each signal is allocated 3 bits, and so can count
    // up to 8, meaning we can pack 21 alerts to fit inside a 64-bit CAN payload.
    if *[warnings.len(), faults.len(), info.len()]
        .iter()
        .max()
        .unwrap()
        > (64 / 3)
    {
        panic!(
            "Number of alerts for node '{}' cannot exceed 21. Check that warnings, faults and infos have at most 21 items each",
            node_name
        );
    }

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

pub fn parse_alert_data(
    can_data_dir: &String,
    node_name: &String,
) -> Option<(Vec<CanMessage>, Vec<CanAlert>)> {
    let file_path = format!("{}/{}/{}_alerts.json", can_data_dir, node_name, node_name);
    let file_content = match std::fs::read_to_string(file_path) {
        Ok(s) => s,
        Err(e) => match e.kind() {
            std::io::ErrorKind::NotFound => return None,
            _ => panic!(
                "Failed to read Alerts JSON file for node {}: {}",
                node_name, e
            ),
        },
    };

    let node_alerts_json_data: AlertsJson = match serde_json::from_str(&file_content) {
        Ok(v) => v,
        Err(e) => panic!(
            "Alerts JSON file is not valid JSON for node {}: {}",
            node_name, e
        ),
    };

    let (
        (
            warnings_msg,
            faults_msg,
            warnings_counts_msg,
            faults_counts_msg,
            info_msg,
            info_counts_msg,
        ),
        (faults_meta_data, warnings_meta_data, info_meta_data),
    ) = parse_node_alerts(node_name, node_alerts_json_data);

    let can_alerts = vec![
        warnings_msg
            .signals
            .iter()
            .map(|signal| {
                let signal_warning_meta = warnings_meta_data
                    .get(&signal.name)
                    .expect("Warning Metadata should be present");
                CanAlert {
                    name: signal.name.clone(),
                    alert_type: CanAlertType::Warning,
                    id: signal_warning_meta.id,
                    description: signal_warning_meta.description.clone(),
                }
            })
            .collect::<Vec<CanAlert>>(),
        faults_msg
            .signals
            .iter()
            .map(|signal| {
                let signal_fault_meta = faults_meta_data
                    .get(&signal.name)
                    .expect("Fault Metadata should be present");
                CanAlert {
                    name: signal.name.clone(),
                    alert_type: CanAlertType::Fault,
                    id: signal_fault_meta.id,
                    description: signal_fault_meta.description.clone(),
                }
            })
            .collect::<Vec<CanAlert>>(),
        info_msg
            .signals
            .iter()
            .map(|signal| CanAlert {
                name: signal.name.clone(),
                alert_type: CanAlertType::Info,
                id: info_meta_data[&signal.name].id,
                description: info_meta_data[&signal.name].description.clone(),
            })
            .collect::<Vec<CanAlert>>(),
    ]
    .into_iter()
    .flatten()
    .collect::<Vec<CanAlert>>();

    return Some((
        vec![
            warnings_msg,
            faults_msg,
            warnings_counts_msg,
            faults_counts_msg,
            info_msg,
            info_counts_msg,
        ],
        can_alerts,
    ));
}
