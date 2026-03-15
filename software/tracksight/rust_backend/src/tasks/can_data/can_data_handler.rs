use std::sync::Arc;

use tokio::{select, spawn};
use tokio::sync::{RwLock, broadcast};
#[allow(unused_imports)]
use colored::Colorize;

use super::influx_handler::run_influx_handler;
use super::live_data_handler::run_live_data_handler;
use crate::health_check::{HealthCheckSender, HealthCheckSenderExt, Task};
use crate::tasks::telem_message::CanPayload;
use crate::tasks::client_api::clients::Clients;
use crate::{vprintln};

use jsoncan_rust::can_database::{CanDatabase, DecodedSignal};

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender,
    mut can_queue_rx: broadcast::Receiver<CanPayload>,
    clients: Arc<RwLock<Clients>>,
    can_db: Arc<CanDatabase>
) {
    vprintln!("{}", "CAN data task started.".yellow());

    let (decoded_signal_tx, _) = broadcast::channel::<DecodedSignal>(32);

    // parsed can signal consumers
    let influx_handler_task: tokio::task::JoinHandle<()> = spawn(run_influx_handler(health_check_tx.clone(), decoded_signal_tx.subscribe()));
    let live_data_handler_task: tokio::task::JoinHandle<()> = spawn(run_live_data_handler(health_check_tx.clone(), decoded_signal_tx.subscribe(), clients));
    
    health_check_tx.send_health_check(Task::CanDataHandler, true).await;
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("CAN data task shutting down.");
                break;
            }
            Ok(can_payload) = can_queue_rx.recv() => {
                let decoded_signals = can_db.unpack(
                    can_payload.can_id, 
                    can_payload.payload, 
                    Some(can_payload.can_timestamp),
                );

                for signal in decoded_signals {
                    if !decoded_signal_tx.send(signal).is_ok() {
                        eprintln!("Parsed can data signal consumers are all closed");
                        break;
                    }
                }
            }
        }
    }
    drop(decoded_signal_tx); // Closing channel signals the other tasks to shutdown
    let _ = influx_handler_task.await;
    let _ = live_data_handler_task.await;
    vprintln!("{}", "CAN data task ended.".yellow());
}

