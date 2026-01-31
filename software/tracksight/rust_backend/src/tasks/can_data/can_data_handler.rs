use std::sync::Arc;

use tokio::{select, spawn};
use tokio::sync::{RwLock, broadcast};

use super::influx_handler::run_influx_handler;
use super::live_data_handler::run_live_data_handler;
use crate::tasks::can_data::load_can_database;
use crate::tasks::telem_message::CanPayload;
use crate::tasks::client_api::clients::Clients;

use jsoncan_rust::can_database::DecodedSignal;

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    mut can_queue_rx: broadcast::Receiver<CanPayload>,
    clients: Arc<RwLock<Clients>>, 
) {
    println!("CAN data handler task started.");

    // load CAN database
    let can_db = load_can_database().expect("Could not init Can db");

    let (decoded_signal_tx, _) = broadcast::channel::<DecodedSignal>(32);

    // parsed can signal consumers
    let influx_handler_task = spawn(run_influx_handler(shutdown_rx.resubscribe(), decoded_signal_tx.subscribe()));
    let live_data_handler_task = spawn(run_live_data_handler(shutdown_rx.resubscribe(), decoded_signal_tx.subscribe(), clients));
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                println!("Shutting down CAN data handler task.");
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
    
    let _ = influx_handler_task.await;
    let _ = live_data_handler_task.await;
    println!("CAN data handler task ended.");
}

