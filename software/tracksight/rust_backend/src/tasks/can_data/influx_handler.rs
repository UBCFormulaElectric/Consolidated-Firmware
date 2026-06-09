use influxdb2::{Client, models::DataPoint};
use tokio::{select, sync::broadcast::{self, Receiver, error::RecvError}};

use crate::{error_println, tasks::can_data::influx_util::{InfluxSignalSource, MAX_BATCH_CAPACITY, build_data_point, flush_buffer}, utils::yellow};
use crate::{config::CONFIG, tasks::{HealthCheckSender, HealthCheckSenderExt, ResultExt, Task}, vprintln};

use jsoncan_rust::can_database::DecodedSignal;


/**
 * After serial_handler parses the can messages,
 * this task consumes the messages and writes them to influxdb
 */
pub async fn run_influx_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
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

    'main: loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Influx handler task shutting down.");
                break;
            }
            ds = decoded_signal_rx.recv() => {
                match ds {
                    Ok(decoded_signal) => {
                        let data = match build_data_point(decoded_signal, InfluxSignalSource::Radio) {
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
                    // Lagging behind is recoverable: drop the missed signals and keep going
                    Err(RecvError::Lagged(n)) => {
                        error_println!("Influx handler lagged, dropped {n} signals");
                        continue;
                    }
                    // Closed channel is the signal to stop thread
                    Err(RecvError::Closed) => {
                        vprintln!("Influx task shutting down.");
                        break 'main;
                    }
                }
            }
        }
    }
    
    flush_buffer(&mut batch_buffer, &influx_client).await;
    
    vprintln!("{}", yellow("Influx task ended."));
}
