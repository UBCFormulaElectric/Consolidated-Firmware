use std::sync::Arc;

use colored::Colorize;
use tokio::sync::{RwLock, broadcast::Receiver};

use crate::tasks::client_api::clients::Clients;

use jsoncan_rust::can_database::DecodedSignal;

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(
    mut can_signals_rx: Receiver<DecodedSignal>,
    clients: Arc<RwLock<Clients>>
) {
    println!("{}", "Live data task started.".yellow());

    loop {
        match can_signals_rx.recv().await {
            Ok(signal) => {
                // todo handle rwlock
                let c = clients.read().await;
                for client_id in c.get_clients_of_signal(&signal.name) {
                    // todo unwrap or handle none case, which would be really weird
                    let _ = c.get_client_socket(&client_id).clone().unwrap().emit(
                        "data", 
                        &serde_json::json!({
                            "name": signal.name,
                            "value": signal.value,
                            "timestamp": signal.timestamp,
                        })
                    );
                }
            }
            // Closed channel or any error is signal to stop thread
            _ => {
                println!("Live data task shutting down.");
                break;
            }
        }
    }

    println!("{}", "Live data task ended.".yellow())
}