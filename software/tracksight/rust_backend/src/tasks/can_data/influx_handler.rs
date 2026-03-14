use colored::Colorize;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use tokio::sync::{broadcast::Receiver};
use futures::stream;

use crate::{config::CONFIG, health_check::{HealthCheckSender, HealthCheckSenderExt, Task}, vprintln};

use jsoncan_rust::can_database::DecodedSignal;

// Write to database once batch size is reached or some termination signal
const MAX_BATCH_CAPACITY: usize = 1000;

/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(
    health_check_tx: HealthCheckSender,
    mut decoded_signal_rx: Receiver<DecodedSignal>
) {
    vprintln!("{}", "Influx task started.".yellow());

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );
    
    // group the data points together until some maximum size, then write to database
    let mut batch_buffer: Vec<DataPoint> = Vec::with_capacity(MAX_BATCH_CAPACITY);
    
    // quick check, fail otherwise
    influx_client.health().await.expect("Unable to connect to InfluxDB");
    
    health_check_tx.send_health_check(Task::InfluxHandler, true).await;

    loop {
        match decoded_signal_rx.recv().await {
            Ok(decoded_signal) => {
                let data = match build_data_point(decoded_signal) {
                    Ok(data) => data,
                    Err(e) => {
                        eprintln!("{e}");
                        continue;
                    } 
                };
                
                batch_buffer.push(data);
                
                if batch_buffer.len() >= MAX_BATCH_CAPACITY {
                    flush_buffer(&mut batch_buffer, &influx_client).await;
                }
            }
            // Closed channel or any error is signal to stop thread
            _ => {
                vprintln!("Influx task shutting down.");
                break;
            }
        }
    }
    
    flush_buffer(&mut batch_buffer, &influx_client).await;
    
    vprintln!("{}", "Influx task ended.".yellow());
}

/**
 * Helper to flush out a buffer
 */
async fn flush_buffer(buffer: &mut Vec<DataPoint>, client: &Client) {
    let body = stream::iter((*buffer).drain(..).collect::<Vec<DataPoint>>());
    let write_res = client.write_with_precision(&CONFIG.influxdb_bucket, body, TimestampPrecision::Milliseconds).await;
    if let Err(e) = write_res {
        eprintln!("Error writing to InfluxDB: {e}");
    }
}

/**
 * helper to build InfluxDB data point
 */
fn build_data_point(decoded_signal: DecodedSignal) -> Result<DataPoint, DataPointError> {
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field(&decoded_signal.name, decoded_signal.value)
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}