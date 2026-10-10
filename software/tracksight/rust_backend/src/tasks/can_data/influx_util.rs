use futures::stream;
use influxdb2::{Client, api::write::TimestampPrecision, models::{DataPoint, data_point::DataPointError}};
use jsoncan_rust::can_database::DecodedSignal;
use serde::{Deserialize, Serialize};
use strum::Display;

use crate::{BOOT_STATE, config::CONFIG, dprintln};
use crate::tasks::can_data::decoded_item::DecodedMarker;

pub const INFLUXDB_SD_DUMPS_MEASUREMENT: &str = "sd_dumps";
pub const INFLUXDB_BOOT_STATE_MEASUREMENT: &str = "boot_state";

#[derive(Debug, Display, Serialize, Deserialize, PartialEq, Eq, Hash, Clone)]
#[strum(serialize_all = "lowercase")]
pub enum InfluxSignalSource {
    Radio,
    SdCard,
}

// Write to database once batch size is reached or some termination signal
pub const MAX_BATCH_CAPACITY: usize = 1024;

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct BootState {
    pub boot_hash: String
}

/**
 * Helper to flush out a buffer
 */
pub async fn flush_buffer(buffer: &mut Vec<DataPoint>, client: &Client) {
    dprintln!("Flushing buffer of size {} to InfluxDB", buffer.len());
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
    let boot_state = BOOT_STATE.get().unwrap().borrow();
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field("_value", decoded_signal.value)
        .tag("signal_name", &decoded_signal.name)
        .tag("source", source.to_string())
        .tag("signal_type", format!("{:?}", decoded_signal.signal_type).to_ascii_lowercase())
        .tag("boot_hash", boot_state.as_ref().map_or_else(|| "", |s| &s.boot_hash))
        .timestamp(decoded_signal.timestamp.unwrap_or_default() as i64)
        .build()
}

pub fn build_marker_data_point(decoded_marker: DecodedMarker, source: InfluxSignalSource) -> Result<DataPoint, DataPointError> {
    let boot_state = BOOT_STATE.get().unwrap().borrow();
    DataPoint::builder(&CONFIG.influxdb_measurement)
        .field("_value", 1.0)
        .tag("signal_name", &decoded_marker.name)
        .tag("source", source.to_string())
        .tag("boot_hash", boot_state.as_ref().map_or_else(|| "", |s| &s.boot_hash))
        .timestamp(decoded_marker.timestamp as i64)
        .build()
}

pub fn build_boot_state_data_point(boot_state: &BootState, source: InfluxSignalSource) -> Result<DataPoint, DataPointError> {
    DataPoint::builder(INFLUXDB_BOOT_STATE_MEASUREMENT)
        .field("_value", 1.0)
        .tag("source", source.to_string())
        .tag("boot_hash", &boot_state.boot_hash)
        .timestamp(chrono::Utc::now().timestamp_millis())
        .build()
}