use tokio::sync::broadcast::Receiver;
use influxdb2::Client;
use tokio::select;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanMessage;

/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(mut shutdown_signal: Receiver<()>, mut can_queue_receiver: Receiver<CanMessage>) {
    println!("Influx task started.");

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );

    loop {
        select! {
            _ = shutdown_signal.recv() => {
                println!("Shutting down influx handler task.");
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