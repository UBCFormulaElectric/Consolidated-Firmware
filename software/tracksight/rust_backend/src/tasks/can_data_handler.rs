
use tokio::sync::broadcast;
use tokio::{select, spawn};

use super::can_data::influx_handler::run_influx_handler;
use super::can_data::live_data_handler::run_live_data_handler;
use super::telem_message::CanPayload;
use crate::config::CONFIG;

use jsoncan_rust::parsing::JsonCanParser;
use jsoncan_rust::can_database::CanDatabase;
use jsoncan_rust::can_database; // todo fix the naming conflict with canmessage bruhhh

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(mut shutdown_rx: broadcast::Receiver<()>, mut can_queue_rx: broadcast::Receiver<CanPayload>) {
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
            }
        }
    }

    // influx_handler_task.await;
    // live_data_handler_task.await;
}

