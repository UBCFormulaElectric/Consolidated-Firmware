use futures::stream;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use jsoncan_rust::can_database::DecodedSignal;
use serde::{Deserialize, Serialize};

use crate::config::CONFIG;

#[derive(Debug, Serialize, Deserialize, PartialEq, Eq, Hash, Clone)]
#[serde(rename_all = "snake_case")]
pub enum InfluxSignalSource {
    Radio,
    SdCard,
}

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
pub fn build_data_point(decoded_signal: DecodedSignal, source: InfluxSignalSource) -> Result<DataPoint, DataPointError> {
    let source_str = serde_json::to_string(&source).unwrap();
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field("_value", decoded_signal.value)
        .tag("signal_name", &decoded_signal.name)
        .tag("source", source_str)
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}