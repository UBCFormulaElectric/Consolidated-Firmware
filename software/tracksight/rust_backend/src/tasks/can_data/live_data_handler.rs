use std::sync::Arc;

use socketioxide::extract::SocketRef;
<<<<<<< HEAD
<<<<<<< HEAD
use tokio::select;
use tokio::sync::broadcast;
=======
>>>>>>> 7ea31ddb6 (Jackyao/paired hotfix (#2054))
=======
use tokio::select;
use tokio::sync::broadcast;
>>>>>>> 4a21fbf61 (i loveee hotfixes (#2062))
use tokio::sync::{RwLock, broadcast::Receiver, broadcast::error::RecvError};

use crate::dprintln;
use crate::utils::yellow;
use crate::{error_println, tasks::{HealthCheckSender, HealthCheckSenderExt, Task, client_api::subtable_clients::Clients}, vprintln};

use jsoncan_rust::can_database::{CanSignalType, DecodedSignal};

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender,
    mut can_signals_rx: Receiver<DecodedSignal>,
    clients: Arc<RwLock<Clients>>
) {
    vprintln!("{}", yellow("Live data task started."));

    health_check_tx.send_health_check(Task::LiveDataHandler, true).await;

    loop {
<<<<<<< HEAD
<<<<<<< HEAD
        select! {
            _ = shutdown_rx.recv() => {
=======
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
                            "signal_type": format!("{:?}", signal.signal_type),
                        })
                    ) {
                        Ok(_) => {}
                        Err(e) => {
                            error_println!("{e}");
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
>>>>>>> 7ea31ddb6 (Jackyao/paired hotfix (#2054))
=======
        select! {
            _ = shutdown_rx.recv() => {
>>>>>>> 4a21fbf61 (i loveee hotfixes (#2062))
                vprintln!("Live data task shutting down.");
                break;
            }
            cs = can_signals_rx.recv() => {
                match cs {
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
                                "signal_type": format!("{:?}", signal.signal_type),
                            })
                        ) {
                            Ok(_) => {}
                            Err(e) => {
<<<<<<< HEAD
                                // error when attempting to send data to client socket,
                                // likely due to client socket connection problems.
                                dprintln!("{e}");
=======
                                error_println!("{e}");
>>>>>>> 4a21fbf61 (i loveee hotfixes (#2062))
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