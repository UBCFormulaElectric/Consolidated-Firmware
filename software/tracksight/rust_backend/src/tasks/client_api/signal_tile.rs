/*
    This is implementation for tiling and caching for signal fetching from InfluxDB
    TBH a lot of these could probably be implemented with a few libraries but this is pretty cool
*/

use std::{pin::Pin, sync::Arc, time::Duration};

use axum::http::StatusCode;
use chrono::{DateTime, FixedOffset, Utc};
use dashmap::DashMap;
use futures::future::{ready, try_join_all};
use influxdb2::{FromDataPoint, RequestError, models::Query};
use serde::Serialize;
use tokio::{select, time::sleep};

use crate::{config::CONFIG, dprintln};

const INFLUX_QUERY_TIMEOUT_S: u64 = 5;

pub type SignalTileCache = Arc<DashMap<SignalTileKey, Vec<SignalTilePoint>>>;

type SignalFuture = Pin<Box<dyn Future<Output = Result<Vec<SignalRow>, RequestError>> + Send>>;

/**
 * The row for each signal value returned from InfluxDB
 */
#[derive(Debug, Serialize, FromDataPoint, Default)]
pub struct SignalRow {
    // rename influx field names to match with frontend names
    #[serde(rename = "timestamp")]
    time: DateTime<FixedOffset>,
    value: f64,
    measurement: String,
    #[serde(rename = "name")]
    signal_name: String,
}


// time interval [a, b] -> bucket sizes/resolution based on max points, round to finer resolution (round down)
// resolution -> tiles (based on predefined buckets (# of points) per tile, hence tilewidth)
// with each tile, check cache
// return data and possibly metadata about tiles and resolution
const WINDOW_SIZE: u64 = 2096; // (min) number of points per query given start and end
const TILE_SIZE: u64 = 512; // number of points per tile

/**
 * Used to group tile data so that strings and resolutions aren't repeated
 */
#[derive(Clone, Hash, Eq, PartialEq)]
pub struct SignalTileKey {
    signal: String,
    tile_start: DateTime<FixedOffset>,
    resolution_ms: u64,
}

/**
 * Values stores in a tile
 */
#[derive(Clone)]
pub struct SignalTilePoint {
    time: DateTime<FixedOffset>,
    value: f64,
}

/**
 * Round the bucket resolution down to a predefined set of resolutions
 * Resolution is in seconds
 * Returns the rounded resolution in seconds and aggregate string for InfluxDB2 query with appropriate units
 */
fn round_resolution_ms(res_ms: f64) -> u64 {
    match res_ms {
        r if r < 100.0 => 10, // 10ms
        r if r < 500.0 => 100, // 100ms
        r if r < 1000.0 => 500, // 500ms
        r if r < 10000.0 => 1000, // 1s
        r if r < 60000.0 => 10000, // 10s
        r if r < 600000.0 => 60000, // 1m
        r if r < 3600000.0 => 600000, // 10m
        r if r < 3600000.0 => 3600000, // 1h
        _ => 3600000, // 1h
    }
}

/**
 * Retrieves data signals from tiles that contains start and end
 * Returns superset of requested time range
 */
pub async fn get_signals(
    influx_client: Arc<influxdb2::Client>, 
    signal_tile_cache: SignalTileCache, 
    signal: String, 
    start: DateTime<Utc>, 
    end: DateTime<Utc>
) -> Result<Vec<SignalRow>, (StatusCode, String)> {
    // resolution in seconds
    let resolution_ms = round_resolution_ms((end - start).as_seconds_f64() * 1000.0 / (WINDOW_SIZE as f64));
    let tile_duration_ms = TILE_SIZE * resolution_ms;
    
    // get tiles
    let mut tiles_str: Vec<DateTime<Utc>> = Vec::new();
    // floor start time to nearest tile duration
    let mut tile_start_utc = 
        match DateTime::from_timestamp_millis(
            start.timestamp_millis() - (start.timestamp_millis() % tile_duration_ms as i64)
        ){
            Some(t) => t,
            None => {
                return Err((StatusCode::INTERNAL_SERVER_ERROR, "Could not find start time for tile".to_string()));
            }
        };
    while tile_start_utc < end {
        tiles_str.push(tile_start_utc);
        tile_start_utc = tile_start_utc + Duration::from_millis(tile_duration_ms);
    }

    let get_tile_query = |tile_start: &DateTime<Utc>| -> String {
        let tile_start_str = tile_start.to_rfc3339_opts(chrono::SecondsFormat::Millis, true);
        format!(r#"
        import "date"
        from(bucket: "{}")
        |> range(start: {tile_start_str}, stop: date.add(d: {tile_duration_ms}ms, to: {tile_start_str}))
        |> aggregateWindow(every: {resolution_ms}ms, fn: mean, createEmpty: false)
        |> filter(fn: (r) => r["_measurement"] == "{}")
        |> filter(fn: (r) => r["signal_name"] == "{signal}")"#
        , &CONFIG.influxdb_bucket, &CONFIG.influxdb_measurement)
    };

    let mut tile_queries: Vec<SignalFuture> = Vec::new();
    for tile_start in &tiles_str {
        let tile_key = SignalTileKey {
            signal: signal.clone(),
            tile_start: (*tile_start).into(),
            resolution_ms
        };

        let cache_fetch = signal_tile_cache.get(&tile_key).map(|r| r.clone());
        match cache_fetch {
            Some(cached_tile) => {
                dprintln!("Cache hit! Tile starting at {} with res {}", 
                tile_start.to_rfc3339_opts(chrono::SecondsFormat::Millis, true), 
                resolution_ms
            );
            // Don't need to asynchronize the transformation process unless tile sizes are super large like 10k
                let signal_rows = cached_tile.into_iter().map(|point| SignalRow {
                    time: point.time,
                    value: point.value,
                    measurement: CONFIG.influxdb_measurement.clone(),
                    signal_name: signal.clone(),
                }).collect::<Vec<SignalRow>>();
                tile_queries.push(Box::pin(ready(Ok(signal_rows))));
            }
            None => {
                dprintln!("Cache miss! Fetching tile starting at {} with res {}", 
                    tile_start.to_rfc3339_opts(chrono::SecondsFormat::Millis, true), 
                    resolution_ms
                );
                let influx_clone = influx_client.clone();
                let cache_clone = signal_tile_cache.clone();
                let tile_query_str = get_tile_query(&tile_start);
                let query = async move {
                    let signal_rows = influx_clone.query::<SignalRow>(
                        Some(Query::new(tile_query_str))
                    ).await?;

                    // If fetch success, insert into cache
                    cache_clone.insert(
                        tile_key, 
                        signal_rows.iter().map(|row| SignalTilePoint {
                            time: row.time,
                            value: row.value,
                        }).collect::<Vec<SignalTilePoint>>()
                    );

                    Ok(signal_rows)
                };
                tile_queries.push(Box::pin(query));
            }
        }
    }

    let result = select! {
        val = try_join_all(tile_queries) => {
            match val {
                Ok(res) => res.into_iter().flatten().collect::<Vec<SignalRow>>(),
                Err(e) => {
                    return Err((StatusCode::INTERNAL_SERVER_ERROR, format!("InfluxDB query error: {e}")));
                }
            }
        },
        _ = sleep(Duration::from_secs(INFLUX_QUERY_TIMEOUT_S)) => {
            return Err((StatusCode::REQUEST_TIMEOUT, "InfluxDB query timed out!".to_string()));
        }
    };
    return Ok(result);
}