use futures::stream;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use jsoncan_rust::can_database::DecodedSignal;
use rmcp::schemars;
use serde::{Deserialize, Serialize};
use strum::{Display, EnumString};

use crate::config::CONFIG;
use crate::tasks::can_data::decoded_item::DecodedMarker;

#[derive(Debug, Display, EnumString, Serialize, Deserialize, PartialEq, Eq, Hash, Clone, schemars::JsonSchema)]
#[strum(serialize_all = "lowercase", ascii_case_insensitive)]
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
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field("_value", decoded_signal.value)
        .tag("signal_name", &decoded_signal.name)
        .tag("source", source.to_string())
        .tag("signal_type", format!("{:?}", decoded_signal.signal_type).to_ascii_lowercase())
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}

pub fn build_marker_data_point(decoded_marker: DecodedMarker, source: InfluxSignalSource) -> Result<DataPoint, DataPointError> {
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field("_value", 1.0)
        .tag("signal_name", &decoded_marker.name)
        .tag("source", source.to_string())
        .timestamp(decoded_marker.timestamp as i64)
        .build()
}