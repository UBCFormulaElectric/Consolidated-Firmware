use std::sync::Arc;

use tokio::{select, spawn};
use tokio::sync::{RwLock, broadcast};

use crate::tasks::can_data::influx_handler::run_influx_handler;
use crate::tasks::can_data::live_data_handler::run_live_data_handler;
use crate::utils::yellow;
use crate::tasks::{HealthCheckSender, HealthCheckSenderExt, ShutdownReceiver, Task};
use crate::tasks::telem_message::CanPayload;
use crate::tasks::client_api::subtable_clients::Clients;
use crate::{error_println, vprintln};

use jsoncan_rust::can_database::{CanDatabase, DecodedSignal};

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(
    mut shutdown_rx: ShutdownReceiver, 
    health_check_tx: HealthCheckSender,
    mut can_queue_rx: broadcast::Receiver<CanPayload>,
    clients: Arc<RwLock<Clients>>,
    can_db: Arc<CanDatabase>
) {
    vprintln!("{}", yellow("CAN data task started."));

    let (decoded_signal_tx, _) = broadcast::channel::<DecodedSignal>(4096);

    // parsed can signal consumers
    let influx_handler_task: tokio::task::JoinHandle<()> = spawn(run_influx_handler(shutdown_rx.resubscribe(), health_check_tx.clone(), decoded_signal_tx.subscribe()));
    let live_data_handler_task: tokio::task::JoinHandle<()> = spawn(run_live_data_handler(shutdown_rx.resubscribe(), health_check_tx.clone(), decoded_signal_tx.subscribe(), clients));
    
    health_check_tx.send_health_check(Task::CanDataHandler, true).await;
    
    'outer: loop {
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
                        error_println!("Parsed can data signal consumers are all closed");
                        break 'outer;
                    }
                }
            }
        }
    }
    drop(decoded_signal_tx); // Closing channel signals the other tasks to shutdown
    let _ = influx_handler_task.await;
    let _ = live_data_handler_task.await;
    vprintln!("{}", yellow("CAN data task ended."));
}

