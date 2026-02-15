
use std::{f64::consts::{PI, TAU}, sync::Arc, time::{Instant, SystemTime, UNIX_EPOCH}};

use jsoncan_rust::can_database::{CanDatabase, DecodedSignal};
use tokio::{select, sync::broadcast::{Receiver, Sender}};

use crate::tasks::telem_message::CanPayload;

/*
    Debugging/dev mock file, not used functionally
*/


pub async fn mock(mut shutdown_rx: Receiver<()>, can_queue_tx: Sender<CanPayload>, can_db: Arc<CanDatabase>) {
    println!("Mock task running...");
    let start = Instant::now();

    loop {
        select! {
            _ = shutdown_rx.recv() => {
                println!("Shutting down mock task.");
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
                    },
                ];
                let (id, payload) = can_db.pack("BMS_TractiveSystem", &signals).unwrap();
                let mock_payload = CanPayload {
                    can_id: id,
                    payload: payload,
                    can_timestamp: SystemTime::now(),
                };
                if !can_queue_tx.send(mock_payload).is_ok() {
                    eprintln!("Channel has closed");
                    return;
                }
                tokio::time::sleep(tokio::time::Duration::from_millis(20)).await;
            } => {}
        }
    }
}