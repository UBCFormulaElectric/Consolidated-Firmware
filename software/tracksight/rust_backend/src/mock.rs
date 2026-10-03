use std::{collections::{HashMap, HashSet}, f64::consts::TAU, sync::Arc, time::{Duration, Instant, SystemTime, UNIX_EPOCH}};

use jsoncan_rust::can_database::{CanDatabase, CanSignalType, DecodedSignal};
use tokio::{select, sync::broadcast, sync::RwLock};

use crate::{tasks::can_data::signal_metadata::{get_all_signal_metadatas, SignalMetadata}, utils::yellow};
use crate::{tasks::{HealthCheckSender, HealthCheckSenderExt, Task}, tasks::telem_message::CanPayload, vprintln};
use crate::Clients;

/*
    Debugging/dev mock file, not used functionally
*/

const MAX_ACTIVE_ALERTS: usize = 10;

const ALERT_RAISE_PROBABILITY: f64 = 0.02;
const ALERT_MIN_DURATION: Duration = Duration::from_secs(1);
const ALERT_MAX_DURATION: Duration = Duration::from_secs(30);

fn update_mock_alerts(
    alert_signals: &[&SignalMetadata],
    active_alerts: &mut HashMap<String, Instant>,
) -> HashSet<String> {
    let now = Instant::now();
    let mut changed_msgs = HashSet::new();

    active_alerts.retain(|alert_name, expiry| {
        let still_active = *expiry > now;
        if !still_active {
            changed_msgs.insert(alert_signals.iter().find(|a| &a.name == alert_name).unwrap().msg_name.clone());
        }
        still_active
    });

    if alert_signals.is_empty()
        || active_alerts.len() >= MAX_ACTIVE_ALERTS
        || !rand::random_bool(ALERT_RAISE_PROBABILITY)
    {
        return changed_msgs;
    }

    let alert = alert_signals[rand::random_range(0..alert_signals.len())];
    if active_alerts.contains_key(&alert.name) {
        return changed_msgs;
    }

    let duration = rand::random_range(ALERT_MIN_DURATION..ALERT_MAX_DURATION);
    active_alerts.insert(alert.name.clone(), now + duration);
    changed_msgs.insert(alert.msg_name.clone());

    changed_msgs
}

/**
 * Mocks serial handler
 */
pub async fn run_mock_task(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender, 
    can_queue_tx: broadcast::Sender<CanPayload>, 
    _diag_tx: broadcast::Sender<f64>,
    can_db: Arc<CanDatabase>,
    clients: Arc<RwLock<Clients>>,
) {
    vprintln!("{}", yellow("Mock task started."));
    let mut i: u32 = 0;

    health_check_tx.send_health_check(Task::SerialHandler, true).await;
    
    let mut diag_interval = tokio::time::interval(tokio::time::Duration::from_secs(1));
    let can_signals = get_all_signal_metadatas(&can_db, None);

    let alert_signals: Vec<&SignalMetadata> = can_signals
        .values()
        .filter(|s| s.signal_type == CanSignalType::Alert)
        .collect();
    let mut alerts_by_msg: HashMap<String, Vec<&SignalMetadata>> = HashMap::new();
    for alert in alert_signals.iter() {
        alerts_by_msg.entry(alert.msg_name.clone()).or_default().push(alert);
    }
    let mut active_alerts: HashMap<String, Instant> = HashMap::new();

    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Mock task shutting down.");
                break;
            }
            _ = diag_interval.tick() => {
                // Simulate a varying error rate between 2.5% and 7.5% for testing
                let simulated_error_rate = 5.0 + (i as f64 * TAU / 50.0).sin() * 2.5;
                _diag_tx.send(simulated_error_rate).ok();
            }
            _ = async {
                // Simulate sending mock CAN payloads
                i += 1;
                let base_value = ((i as f64 * TAU / 100.0).sin() + 1.0) / 2.0;

                if i == 1 {
                    let bootup_signal = DecodedSignal {
                        name: "DAM_Alive".to_string(),
                        value: 1.0,
                        timestamp: None,
                        label: None,
                        unit: None,
                        signal_type: CanSignalType::Boolean
                    };

                    let (id, payload) = can_db.pack("DAM_Bootup", &vec!(bootup_signal)).unwrap();
                    let mock_payload = CanPayload {
                        can_id: id,
                        payload: payload,
                        can_timestamp: SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_millis() as u64,
                    };

                    if let Err(e) = can_queue_tx.send(mock_payload) {
                        panic!("can_queue_tx send error: {}", e);
                    }
                }

                let mut alert_msgs = update_mock_alerts(&alert_signals, &mut active_alerts);
                alert_msgs.extend(
                    active_alerts.keys().map(|name| can_signals[name].msg_name.clone())
                );

                let subscribed_signals = clients.read().await.get_subscribed_signals();
                let mut signals_by_msg: HashMap<String, Vec<DecodedSignal>> = HashMap::new();

                for signal_name in subscribed_signals {
                    let Some(signal_metadata) = can_signals.get(&signal_name) else {
                        continue;
                    };
                    
                    if signal_metadata.signal_type == CanSignalType::Alert {
                        continue;
                    }
                    
                    let value = base_value * (signal_metadata.max_val - signal_metadata.min_val) + signal_metadata.min_val;
                    let is_discrete = signal_metadata.enum_signal.is_some() || (signal_metadata.min_val == 0.0 && signal_metadata.max_val == 1.0);

                    signals_by_msg
                        .entry(signal_metadata.msg_name.clone())
                        .or_insert_with(Vec::new)
                        .push(DecodedSignal {
                            name: signal_metadata.name.clone(),
                            value: if is_discrete {
                                value.round()
                            } else {
                                value
                            },
                            timestamp: None,
                            label: None,
                            unit: None,
                            signal_type: if is_discrete {
                                CanSignalType::Enum
                            } else {
                                CanSignalType::Numerical
                            }
                        });
                }

                for msg_name in alert_msgs {
                    signals_by_msg.entry(msg_name).or_default();
                }

                for (msg_name, signals) in signals_by_msg.iter_mut() {
                    let Some(alerts) = alerts_by_msg.get(msg_name) else {
                        continue;
                    };

                    signals.extend(alerts.iter().map(|alert| DecodedSignal {
                        name: alert.name.clone(),
                        value: if active_alerts.contains_key(&alert.name) { 1.0 } else { 0.0 },
                        timestamp: None,
                        label: None,
                        unit: None,
                        signal_type: CanSignalType::Alert,
                    }));
                }

                for (msg_name, signals) in signals_by_msg {
                    let (id, payload) = can_db.pack(&msg_name, &signals).unwrap();
                    let mock_payload = CanPayload {
                        can_id: id,
                        payload: payload,
                        can_timestamp: SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_millis() as u64,
                    };

                    if let Err(e) = can_queue_tx.send(mock_payload) {
                        panic!("can_queue_tx send error: {}", e);
                    }
                }

                tokio::time::sleep(tokio::time::Duration::from_millis(20)).await;
            } => {}
        }
    }

    vprintln!("{}", yellow("Mock task ended."));
}