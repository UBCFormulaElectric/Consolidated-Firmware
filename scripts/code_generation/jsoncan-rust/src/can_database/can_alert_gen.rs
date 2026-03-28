use crate::can_database::{
    CanAlert, CanAlertType, CanBusModes, CanMessage, CanSignal, CanSignalType, NodeAlerts,
};

fn parse_node_alert_signals(
    node_name: &String,
    alerts: &Vec<CanAlert>,
    alert_type: CanAlertType,
) -> (Vec<CanSignal>, Vec<CanSignal>) {
    // From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
    // :returns metadata which maps names of alert signals to their AlertsEntry, list of CanSignals for the alert message

    let mut signals: Vec<CanSignal> = Vec::new();
    let mut signals_counts: Vec<CanSignal> = Vec::new();
    let mut bit_pos: u16 = 0;

    for alert in alerts {
        let alert_type_name: &str = match alert_type {
            CanAlertType::Warning => "Warning",
            CanAlertType::Fault => "Fault",
            CanAlertType::Info => "Info",
        };

        signals.push(CanSignal {
            name: format!("{}_{}_{}", node_name, alert_type_name, alert.name),
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
        signals_counts.push(CanSignal {
            name: format!("{}_{}_{}_Count", node_name, alert_type_name, alert.name),
            start_bit: bit_pos,
            bits: 8,
            scale: 1.0,
            offset: 0.0,
            min: 0.0,
            max: 255.0,
            start_val: 0.0,
            enum_name: None,
            unit: None,
            signed: false,
            description: None,
            big_endian: false,
            signal_type: CanSignalType::Numerical,
        });

        bit_pos += 1;
    }

    (signals, signals_counts)
}

pub fn generate_node_alert_msgs(node_name: &String, alerts_json: &NodeAlerts) -> [CanMessage; 6] {
    // Check alert messages ID are unique
    let warnings_name = format!("{}_Warnings", node_name);
    let faults_name = format!("{}_Faults", node_name);
    let info_name = format!("{}_Info", node_name);
    let warnings_counts_name = format!("{}_WarningsCounts", node_name);
    let faults_counts_name = format!("{}_FaultsCounts", node_name);
    let info_counts_name = format!("{}_InfoCounts", node_name);

    // Make alert signals
    let (warnings_signals, warnings_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.warnings, CanAlertType::Warning);
    let (faults_signals, faults_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.faults, CanAlertType::Fault);
    let (info_signals, info_counts_signals) =
        parse_node_alert_signals(node_name, &alerts_json.infos, CanAlertType::Info);

    static WARNINGS_ALERTS_CYCLE_TIME: Option<u32> = Some(100);
    static FAULTS_ALERTS_CYCLE_TIME: Option<u32> = Some(100);
    static INFO_ALERTS_CYCLE_TIME: Option<u32> = Some(100);

    [
        CanMessage {
            name: info_name,
            id: alerts_json.infos_id,
            description: Some(format!("Status of info for the {}.", node_name)),
            cycle_time: INFO_ALERTS_CYCLE_TIME,
            log_cycle_time: INFO_ALERTS_CYCLE_TIME,
            telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
            signals: info_signals,
            tx_node_name: node_name.clone(),
            modes: CanBusModes::All,
        },
        CanMessage {
            name: warnings_name,
            id: alerts_json.warnings_id,
            description: Some(format!("Status of warnings for the {}.", node_name)),
            cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            signals: warnings_signals,
            tx_node_name: node_name.clone(),
            modes: CanBusModes::All,
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
            modes: CanBusModes::All,
        },
        CanMessage {
            name: info_counts_name,
            id: alerts_json.infos_count_id,
            description: Some(format!(
                "Number of times info have been set for the {}.",
                node_name
            )),
            cycle_time: INFO_ALERTS_CYCLE_TIME,
            log_cycle_time: INFO_ALERTS_CYCLE_TIME,
            telem_cycle_time: INFO_ALERTS_CYCLE_TIME,
            signals: info_counts_signals,
            tx_node_name: node_name.clone(),
            modes: CanBusModes::All,
        },
        CanMessage {
            name: warnings_counts_name,
            id: alerts_json.warnings_count_id,
            description: Some(format!(
                "Number of times warnings have been set for the {}.",
                node_name
            )),
            cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            log_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            telem_cycle_time: WARNINGS_ALERTS_CYCLE_TIME,
            signals: warnings_counts_signals,
            tx_node_name: node_name.clone(),
            modes: CanBusModes::All,
        },
        CanMessage {
            name: faults_counts_name,
            id: alerts_json.faults_count_id,
            description: Some(format!(
                "Number of times faults have been set for the {}.",
                node_name
            )),
            cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            log_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            telem_cycle_time: FAULTS_ALERTS_CYCLE_TIME,
            signals: faults_counts_signals,
            tx_node_name: node_name.clone(),
            modes: CanBusModes::All,
        },
    ]
}
