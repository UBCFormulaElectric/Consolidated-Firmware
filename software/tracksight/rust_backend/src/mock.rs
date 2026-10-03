use std::{collections::HashMap, f64::consts::TAU, sync::Arc, time::{SystemTime, UNIX_EPOCH}};

use jsoncan_rust::can_database::{CanDatabase, CanSignalType, DecodedSignal};
use tokio::{select, sync::broadcast, sync::RwLock};

use crate::{tasks::can_data::signal_metadata::get_all_signal_metadatas, utils::yellow};
use crate::{tasks::{HealthCheckSender, HealthCheckSenderExt, Task}, tasks::telem_message::CanPayload, vprintln};
use crate::Clients;

/*
    Debugging/dev mock file, not used functionally
*/

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

                if base_value >= 0.5 {
                
                }

                let subscribed_signals = clients.read().await.get_subscribed_signals();
                let mut signals_by_msg: HashMap<String, Vec<DecodedSignal>> = HashMap::new();

                for signal_name in subscribed_signals {
                    let Some(signal_metadata) = can_signals.get(&signal_name) else {
                        continue;
                    };
                    
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