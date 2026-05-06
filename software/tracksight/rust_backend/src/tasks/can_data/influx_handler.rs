use influxdb2::{Client, models::DataPoint};
use tokio::sync::{broadcast::Receiver};

use crate::{tasks::can_data::influx_util::{MAX_BATCH_CAPACITY, build_data_point, flush_buffer}, utils::yellow};
use crate::{config::CONFIG, tasks::{HealthCheckSender, HealthCheckSenderExt, ResultExt, Task}, vprintln};

use jsoncan_rust::can_database::DecodedSignal;


/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(
    health_check_tx: HealthCheckSender,
    mut decoded_signal_rx: Receiver<DecodedSignal>
) {
    vprintln!("{}", yellow("Influx task started."));

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );
    
    // group the data points together until some maximum size, then write to database
    let mut batch_buffer: Vec<DataPoint> = Vec::with_capacity(MAX_BATCH_CAPACITY);
    
    // quick check, fail otherwise
    influx_client.health().await.unwrap_or_fail_health_check(&health_check_tx, Task::InfluxHandler).await;
    
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
    
    vprintln!("{}", yellow("Influx task ended."));
}
