use colored::Colorize;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use tokio::{select, sync::{broadcast::Receiver}};
use futures::stream;

use crate::config::CONFIG;

use jsoncan_rust::can_database::DecodedSignal;

// Write to database once batch size is reached or some termination signal
const MAX_BATCH_CAPACITY: usize = 1000;

/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(mut shutdown_signal: Receiver<()>, mut decoded_signal_rx: Receiver<DecodedSignal>) {
    println!("{}", "Influx task started.".yellow());

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );

    // group the data points together until some maximum size, then write to database
    let mut batch_buffer: Vec<DataPoint> = Vec::with_capacity(MAX_BATCH_CAPACITY);

    loop {
        select! {
            _ = shutdown_signal.recv() => {
                println!("Influx task shutting down.");
                break;
            }
            Ok(decoded_signal) = decoded_signal_rx.recv() => {
                // todo should also probably check for closed channels and close thread
                let data = match build_data_point(decoded_signal) {
                    Ok(data) => data,
                    Err(e) => {
                        eprintln!("{e}");
                        break;
                    } 
                };

                batch_buffer.push(data);

                if batch_buffer.len() >= MAX_BATCH_CAPACITY {
                    let body = stream::iter(batch_buffer.drain(..).collect::<Vec<DataPoint>>());
                    influx_client.write_with_precision(&CONFIG.influxdb_bucket, body, TimestampPrecision::Milliseconds);
                }
            }
        }
    }

    println!("{}", "Influx task ended.".yellow());
}

fn build_data_point(decoded_signal: DecodedSignal) -> Result<DataPoint, DataPointError> {
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field(&decoded_signal.name, decoded_signal.value)
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}