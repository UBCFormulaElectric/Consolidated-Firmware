
use std::{f64::consts::{TAU}, sync::Arc, time::{Instant, SystemTime, UNIX_EPOCH}};

use colored::Colorize;
use jsoncan_rust::can_database::{CanDatabase, CanSignalType, DecodedSignal};
use tokio::{select, sync::broadcast};

use crate::{health_check::{HealthCheckSender, HealthCheckSenderExt, Task}, tasks::telem_message::CanPayload, vprintln};

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
    vprintln!("{}", "Mock task started.".yellow());
    let start = Instant::now();

    health_check_tx.send_health_check(Task::SerialHandler, true).await;
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Mock task shutting down.");
                break;
            }
            _ = async {
                // Simulate sending mock CAN payloads
                let value = (start.elapsed().as_secs_f64() * TAU).sin();
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
                    can_timestamp: SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_millis() as u32,
                };
                if !can_queue_tx.send(mock_payload).is_ok() {
                    eprintln!("Channel has closed");
                    return;
                }
                tokio::time::sleep(tokio::time::Duration::from_millis(20)).await;
            } => {}
        }
    }

    vprintln!("{}", "Mock task ended.".yellow());
}