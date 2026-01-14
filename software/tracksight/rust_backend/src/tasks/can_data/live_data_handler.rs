use tokio::sync::broadcast::Receiver;
use influxdb2::Client;
use tokio::select;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanPayload;

use jsoncan_rust::can_database::CanMessage;

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(mut shutdown_signal: Receiver<()>, mut can_signals_rx: Receiver<()>) {
    println!("Live data task started.");

    loop {
        select! {
            _ = shutdown_signal.recv() => {
                println!("Shutting down live data handler task.");
                break;
            }
            _ = can_signals_rx.recv() => {
                // todo should also probably check for closed channels and close thread
                // todo handle
            }
        }
    }
}