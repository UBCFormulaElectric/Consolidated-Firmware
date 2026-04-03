use std::sync::Arc;

use socketioxide::extract::SocketRef;
use tokio::sync::{RwLock, broadcast::Receiver};

#[allow(unused_imports)]
use crate::utils::yellow;
use crate::{error_println, tasks::{HealthCheckSender, HealthCheckSenderExt, Task, client_api::subtable_clients::Clients}, vprintln};

use jsoncan_rust::can_database::{CanSignalType, DecodedSignal};

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(
    health_check_tx: HealthCheckSender,
    mut can_signals_rx: Receiver<DecodedSignal>,
    clients: Arc<RwLock<Clients>>
) {
    vprintln!("{}", yellow("Live data task started."));

    health_check_tx.send_health_check(Task::LiveDataHandler, true).await;

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
                    match socket.emit(
                        "data", 
                        &serde_json::json!({
                            "name": signal.name,
                            "value": signal.value,
                            "timestamp": signal.timestamp,
                        })
                    ) {
                        Ok(_) => {}
                        Err(e) => {
                            error_println!("{e}");
                        }
                    }
                }
            }
            // Closed channel or any error is signal to stop thread
            _ => {
                vprintln!("Live data task shutting down.");
                break;
            }
        }
    }

    vprintln!("{}", yellow("Live data task ended."));
}