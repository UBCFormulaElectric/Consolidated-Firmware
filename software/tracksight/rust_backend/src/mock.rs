
use std::{f64::consts::{TAU}, sync::Arc, time::{SystemTime, UNIX_EPOCH}};

use jsoncan_rust::can_database::{CanDatabase, CanSignalType, DecodedSignal};
use tokio::{select, sync::broadcast};

#[allow(unused_imports)]
use crate::utils::yellow;
use crate::{tasks::{HealthCheckSender, HealthCheckSenderExt, Task}, tasks::telem_message::CanPayload, vprintln};

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
    can_db: Arc<CanDatabase>
) {
    vprintln!("{}", yellow("Mock task started."));
    let mut i: u32 = 0;

    health_check_tx.send_health_check(Task::SerialHandler, true).await;
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Mock task shutting down.");
                break;
            }
            _ = async {
                // Simulate sending mock CAN payloads
                i += 1;
                let value = (i as f64 * TAU/100.0).sin() * 10.0 + 10.0;
                let signals = vec![
                    DecodedSignal {
                        name: "BMS_TractiveSystemVoltage".to_string(),
                        value: value,
                        timestamp: None,
                        label: None,
                        unit: None,
                        signal_type: CanSignalType::Numerical
                    },
                ];
                let (id, payload) = can_db.pack("BMS_TractiveSystem", &signals).unwrap();
                let mock_payload = CanPayload {
                    can_id: id,
                    payload: payload,
                    can_timestamp: SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_millis() as u64,
                };
                if let Err(e) = can_queue_tx.send(mock_payload) {
                    panic!("can_queue_tx send error: {}", e);
                }
                tokio::time::sleep(tokio::time::Duration::from_millis(20)).await;
            } => {}
        }
    }

    vprintln!("{}", yellow("Mock task ended."));
}