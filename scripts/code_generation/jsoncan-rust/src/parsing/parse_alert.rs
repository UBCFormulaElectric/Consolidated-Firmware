use crate::can_database::{CanAlert, CanAlertType};
use std::collections::HashMap;

#[derive(serde::Deserialize)]
struct AlertEntrySchema {
    id: u32,
    description: String,
    #[serde(default)]
    disabled: bool,
}

#[derive(serde::Deserialize)]
struct AlertsSchema {
    warnings_id: u32,
    warnings_counts_id: u32,
    faults_id: u32,
    faults_counts_id: u32,
    info_id: u32,
    info_counts_id: u32,
    warnings: HashMap<String, AlertEntrySchema>,
    faults: HashMap<String, AlertEntrySchema>,
    info: HashMap<String, AlertEntrySchema>,
}

#[derive(Clone)]
pub struct JsonAlert {
    pub id: u32,
    pub count_id: u32,
    pub alerts: Vec<CanAlert>,
}

#[derive(Clone)]
pub struct JsonAlerts {
    pub infos: JsonAlert,
    pub warnings: JsonAlert,
    pub faults: JsonAlert,
}

pub fn parse_alert_data(can_data_dir: &String, node_name: &String) -> Option<JsonAlerts> {
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

    let value: serde_json::Value = serde_json::from_str(&file_content)
        .expect(&format!("Failed to parse Alerts JSON file for node {}", node_name));

    if value == serde_json::json!({}) {
        return None;
    }

    let alerts_json: AlertsSchema = match serde_json::from_value(value) {
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

    Some(JsonAlerts {
        infos: JsonAlert {
            id: alerts_json.info_id,
            count_id: alerts_json.info_counts_id,
            alerts: info,
        },
        warnings: JsonAlert {
            id: alerts_json.warnings_id,
            count_id: alerts_json.warnings_counts_id,
            alerts: warnings,
        },
        faults: JsonAlert {
            id: alerts_json.faults_id,
            count_id: alerts_json.faults_counts_id,
            alerts: faults,
        },
    })
}
