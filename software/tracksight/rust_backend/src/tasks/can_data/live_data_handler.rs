use std::sync::Arc;

use socketioxide::extract::SocketRef;
use tokio::select;
use tokio::sync::broadcast;
use tokio::sync::{RwLock, broadcast::Receiver, broadcast::error::RecvError};

use crate::dprintln;
use crate::utils::yellow;
use crate::{error_println, tasks::{HealthCheckSender, HealthCheckSenderExt, Task, client_api::subtable_clients::Clients}, vprintln};
use crate::tasks::can_data::decoded_item::DecodedItem;

use jsoncan_rust::can_database::CanSignalType;

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender,
    mut can_signals_rx: Receiver<DecodedItem>,
    clients: Arc<RwLock<Clients>>
) {
    vprintln!("{}", yellow("Live data task started."));

    health_check_tx.send_health_check(Task::LiveDataHandler, true).await;

    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Live data task shutting down.");
                break;
            }
            cs = can_signals_rx.recv() => {
                match cs {
                    Ok(item) => {
                        // todo handle rwlock
                        let rwlock_clients = clients.read().await;

                        let (socket_payload, clients): (serde_json::Value, Vec<String>) = match item {
                            DecodedItem::Signal(signal) => {
                                // if alert type, get all clients
                                let clients = match signal.signal_type {
                                    CanSignalType::Alert => rwlock_clients.get_clients(),
                                    _ => rwlock_clients.get_clients_of_signal(&signal.name),
                                };
                                (
                                    serde_json::json!({
                                        "name": signal.name,
                                        "value": signal.value,
                                        "timestamp": signal.timestamp,
                                        "signal_type": format!("{:?}", signal.signal_type),
                                    }),
                                    clients,
                                )
                            }
                            DecodedItem::Marker(marker) => (
                                serde_json::json!({
                                    "name": marker.name,
                                    "value": 1.0,
                                    "timestamp": marker.timestamp,
                                    "signal_type": "Marker",
                                }),
                                rwlock_clients.get_clients(),
                            ),
                        };

                        let client_sockets: Vec<&SocketRef> = clients
                            .iter()
                            .filter_map(|c| rwlock_clients.get_client_socket(c))
                            .collect();

                        for socket in client_sockets {
                            match socket.emit("data", &socket_payload) {
                                Ok(_) => {}
                                Err(e) => {
                                    // likely due to client socket connection problems.
                                    dprintln!("{e}");
                                }
                            }
                        }
                    }
                // Lagging behind is recoverable: drop the missed signals and keep going
                Err(RecvError::Lagged(n)) => {
                    error_println!("Live data handler lagged, dropped {n} signals");
                    continue;
                }
                // Closed channel is the signal to stop thread
                Err(RecvError::Closed) => {
                    vprintln!("Live data task shutting down.");
                    break;
                }
            }
        }
    }
    }

    vprintln!("{}", yellow("Live data task ended."));
}