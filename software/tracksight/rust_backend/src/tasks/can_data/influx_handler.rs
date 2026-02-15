use influxdb2::models::DataPoint;
use tokio::sync::broadcast::Receiver;
use influxdb2::Client;
use tokio::select;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanPayload;

use jsoncan_rust::can_database::{CanMessage, DecodedSignal};

/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(mut shutdown_signal: Receiver<()>, mut can_queue_receiver: Receiver<DecodedSignal>) {
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
            _ = can_queue_receiver.recv() => {
                // todo should also probably check for closed channels and close thread
                // let CanMessage { can_id, can_time_offset, can_payload } = can;
                // let influx_data = vec![
                //     DataPoint::builder(&CONFIG.influxdb_measurement)
                //         .tag("can_id", can_id.to_string())
                //         .field("can_time_offset", can_time_offset as i64)
                //         .field("can_payload", can_payload.iter().map(|b| *b as i64).collect::<Vec<i64>>())
                //         .build().unwrap()
                // ];
                // influx_client.write(&CONFIG.influxdb_bucket,);
            }
        }
    }
}