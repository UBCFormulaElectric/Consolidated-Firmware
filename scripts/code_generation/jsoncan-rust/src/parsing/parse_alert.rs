use crate::can_database::{CanAlert, CanAlertType};
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
    warnings: HashMap<String, AlertMetaData>,
    faults: HashMap<String, AlertMetaData>,
    info: HashMap<String, AlertMetaData>,
}

pub fn parse_alert_data(can_data_dir: &String, node_name: &String) -> Option<Vec<CanAlert>> {
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

    let alerts_json: AlertsJson = match serde_json::from_str(&file_content) {
        Ok(v) => v,
        Err(e) => panic!(
            "Alerts JSON file is not valid JSON for node {}: {}",
            node_name, e
        ),
    };

    // Parse JSON data dictionary representing a node's alerts.
    let info: Vec<CanAlert> = alerts_json
        .info
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .map(|(name, meta)| CanAlert {
            name,
            alert_type: CanAlertType::Fault,
            id: meta.id,
            description: meta.description,
        })
        .collect();
    let warnings: Vec<CanAlert> = alerts_json
        .warnings
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .map(|(name, meta)| CanAlert {
            name,
            alert_type: CanAlertType::Warning,
            id: meta.id,
            description: meta.description,
        })
        .collect();
    let faults: Vec<CanAlert> = alerts_json
        .faults
        .into_iter()
        .filter(|(_, alert)| !alert.disabled)
        .map(|(name, meta)| CanAlert {
            name,
            alert_type: CanAlertType::Fault,
            id: meta.id,
            description: meta.description,
        })
        .collect();

    // Number of alerts can't exceed 21. This is because we transmit a "counts" message for faults and warnings
    // that indicate the number of times an alert has been set. Each signal is allocated 3 bits, and so can count
    // up to 8, meaning we can pack 21 alerts to fit inside a 64-bit CAN payload.
    if warnings.len() > 64 / 3 {
        panic!(
            "Number of warning alerts for node '{}' cannot exceed 21. Check that warnings have at most 21 items",
            node_name
        );
    }
    if faults.len() > 64 / 3 {
        panic!(
            "Number of fault alerts for node '{}' cannot exceed 21. Check that faults have at most 21 items",
            node_name
        );
    }
    if info.len() > 64 / 3 {
        panic!(
            "Number of info alerts for node '{}' cannot exceed 21. Check that infos have at most 21 items",
            node_name
        );
    }

    let mut can_alerts: Vec<CanAlert> = vec![];
    can_alerts.extend(faults);
    can_alerts.extend(warnings);
    can_alerts.extend(info);

    return Some(can_alerts);
}
