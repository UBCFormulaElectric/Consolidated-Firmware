use std::sync::Arc;

use colored::Colorize;
use socketioxide::extract::SocketRef;
use tokio::sync::{RwLock, broadcast::Receiver};

use crate::tasks::client_api::clients::Clients;

use jsoncan_rust::can_database::{CanSignalType, DecodedSignal};

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
                let rwlock_clients = clients.read().await;

                // if alert type, get all clients
                let clients: Vec<String> = match signal.signal_type {
                    CanSignalType::Alert => rwlock_clients.get_clients(),
                    _ => rwlock_clients.get_clients_of_signal(&signal.name)
                };

                let client_sockets: Vec<&SocketRef> = clients.iter()
                    .filter_map(|c| rwlock_clients.get_client_socket(c))
                    .collect();

                for socket in client_sockets {
                    // todo unwrap or handle none case, which would be really weird
                    let _ = socket.emit(
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