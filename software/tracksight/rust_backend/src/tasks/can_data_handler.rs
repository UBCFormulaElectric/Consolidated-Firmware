
use tokio::{select};
use tokio::sync::{RwLock, broadcast};

use super::can_data::influx_handler::run_influx_handler;
use super::can_data::live_data_handler::run_live_data_handler;
use super::telem_message::CanPayload;
use crate::config::CONFIG;
use crate::tasks::api::subscriptions::Subscriptions;

use jsoncan_rust::parsing::JsonCanParser;
use jsoncan_rust::can_database::CanDatabase;

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    mut can_queue_rx: broadcast::Receiver<CanPayload>,
    subscriptions: RwLock<Subscriptions>
) {
    println!("CAN data handler task started.");

    let parser = JsonCanParser::new(CONFIG.jsoncan_config_path.clone());
    let can_db = match CanDatabase::from(parser) {
        Ok(db) => db,
        Err(e) => {
            eprintln!("Failed to load CAN database: {:?}", e);
            return;
        }
    };

    // let (parsed_can_tx, _) = broadcast::channel::<can_database::CanMessage>(32);

    // parsed can signal consumers
    // let influx_handler_task = spawn(run_influx_handler(shutdown_rx.resubscribe(), parsed_can_tx.subscribe()));
    // let live_data_handler_task = spawn(run_live_data_handler(shutdown_rx.resubscribe(), parsed_can_tx.subscribe()));
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                println!("Shutting down CAN data handler task.");
                break;
            }
            Ok(can_payload) = can_queue_rx.recv() => {
                // let can_message = match can_db.get_message_by_id(can_payload.can_id) {
                //     Ok(c) => c,
                //     Err(_) => {
                //         eprintln!("Unknown CAN ID: {:X}", can_payload.can_id);
                //         continue;
                //     },
                // };

                let decoded_signals = can_db.unpack(
                    can_payload.can_id, 
                    can_payload.payload, 
                    Some(can_payload.can_timestamp),
                );

                for signal in decoded_signals {
                    for client_id in subscriptions.read().await.get_clients_of_signal(signal.name) {
                        // todo emit to client
                    }
                }
            }
        }
    }

    // influx_handler_task.await;
    // live_data_handler_task.await;
}

