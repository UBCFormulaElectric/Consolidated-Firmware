use std::sync::Arc;

use socketioxide::extract::SocketRef;
use tokio::sync::{RwLock, broadcast::Receiver};
#[allow(unused_imports)]
use colored::Colorize;

use crate::{health_check::{HealthCheckSender, HealthCheckSenderExt, Task}, tasks::client_api::clients::Clients, vprintln};

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
    vprintln!("{}", "Live data task started.".yellow());

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
                vprintln!("Live data task shutting down.");
                break;
            }
        }
    }

    vprintln!("{}", "Live data task ended.".yellow())
}