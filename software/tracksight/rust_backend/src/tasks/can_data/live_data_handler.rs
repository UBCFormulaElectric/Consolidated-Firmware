use tokio::sync::broadcast::Receiver;
use influxdb2::Client;
use tokio::select;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanMessage;

/**
 * After serial_handler parses the can messages,
 * this task will handle "forwarding" live data to clients via sockets
 */
pub async fn run_live_data_handler(mut shutdown_signal: Receiver<()>, mut can_queue_receiver: Receiver<CanMessage>) {
    println!("Live data task started.");

    loop {
        select! {
            _ = shutdown_signal.recv() => {
                println!("Shutting down live data handler task.");
                break;
            }
            Ok(can) = can_queue_receiver.recv() => {
                // todo should also probably check for closed channels and close thread
                let CanMessage { can_id, can_time_offset, can_payload } = can;
                // todo handle
            }
        }
    }
}