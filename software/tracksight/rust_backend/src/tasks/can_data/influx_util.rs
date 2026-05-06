use futures::stream;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use jsoncan_rust::can_database::DecodedSignal;

use crate::config::CONFIG;

// Write to database once batch size is reached or some termination signal
pub const MAX_BATCH_CAPACITY: usize = 500;

/**
 * Helper to flush out a buffer
 */
pub async fn flush_buffer(buffer: &mut Vec<DataPoint>, client: &Client) {
    let body = stream::iter((*buffer).drain(..).collect::<Vec<DataPoint>>());
    let write_res = client.write_with_precision(&CONFIG.influxdb_bucket, body, TimestampPrecision::Milliseconds).await;
    if let Err(e) = write_res {
        eprintln!("Error writing to InfluxDB: {e}");
    }
}

/**
 * helper to build InfluxDB data point
 */
pub fn build_data_point(decoded_signal: DecodedSignal) -> Result<DataPoint, DataPointError> {
    DataPoint::builder(&CONFIG.influxdb_measurement_radio)
        .field("_value", decoded_signal.value)
        .tag("signal_name", &decoded_signal.name)
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}