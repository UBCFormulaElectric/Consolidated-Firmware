use std::{collections::HashMap, mem, time::{Duration, SystemTime}};

use axum::{Json, Router, extract::{Path, Query, State}, http::StatusCode, response::IntoResponse, routing::get};
use jsoncan_rust::can_database::CanMessage;
use serde::Serialize;
use regex::Regex;
use serde_json::from_str;
use tokio::{select, time::sleep};

use crate::{config::CONFIG, dprintln, tasks::{can_data::influx_util::InfluxSignalSource, client_api::{AppState, signal_tile::{InfluxSignalRow, get_signals}}}, utils::{rfc3339_to_utc, rfc3339_to_utc_str}};
use crate::tasks::client_api::INFLUX_QUERY_TIMEOUT_MS;

/**
 * Gets the list of all nodes (str) in the current parser.
 */
async fn nodes(State(state): State<AppState>) -> impl IntoResponse {
    let nodes = state.can_db.nodes
        .iter().map(|node| node.name.clone())
        .collect::<Vec<_>>();

    return (StatusCode::OK, Json(nodes));
}

#[derive(Debug, Serialize)]
struct SignalMetadata {
    name: String,
    min_val: f64,
    max_val: f64,
    unit: Option<String>,
    enum_signal: Option<SignalMetadataEnumSignal>,
    tx_node: String,
    cycle_time_ms: Option<u32>,
    id: u32,
    msg_name: String,
}

#[derive(Debug, Serialize)]
struct SignalMetadataEnumSignal {
    enum_name: String,
    enum_values: HashMap<String, u32>,
}

/**
 * Gets metadata of the signal of the current parser.
 * Pass signal names (regex) in `name` parameter.
 * E.g. `/signal/metadata?name=INVFR_bError`
 */
async fn metadata(Query((mut name,)): Query<(Option<String>,)>, State(state): State<AppState>) -> impl IntoResponse {
    let regex = match Regex::new(name.get_or_insert(".*".to_string())) {
        Ok(regex) => regex,
        Err(_) => {
            return (StatusCode::BAD_REQUEST, Json::default());
        }
    };

    let flat_map = | msg: &CanMessage | {
        return msg.signals.iter().map(
            | signal | {
                let can_enum: Option<SignalMetadataEnumSignal> = 
                    if let Some(enum_name) = &signal.enum_name && 
                    let Some(can_enum) = 
                    state.can_db.get_enum(enum_name) {
                        Some(SignalMetadataEnumSignal {
                            enum_name: can_enum.name.clone(),
                            enum_values: can_enum.values.clone()
                        })
                    } else {
                        None
                    };
                (signal.name.clone(),
                SignalMetadata {
                    name: signal.name.clone(),
                    min_val: signal.min,
                    max_val: signal.max,
                    unit: signal.unit.clone(),
                    enum_signal: can_enum,
                    tx_node: msg.tx_node_name.clone(),
                    cycle_time_ms: msg.cycle_time.clone(),
                    id: msg.id,
                    msg_name: msg.name.clone(),
                })
            }
        ).collect::<Vec<_>>()
    };

    let metadatas: HashMap<String, SignalMetadata> = state.can_db.get_all_msgs()
        .unwrap_or_default()
        .iter()
        .filter(|msg| regex.is_match(&msg.name))
        .flat_map(flat_map)
        .collect();

    return (StatusCode::OK, Json(metadatas));
}

/**
 * Gets signal values, timestamp, and name.
 * Format date as RFC3339 (i.e. YYYY-MM-DDTHH:mm:ssZ or YYYY-MM-DDTHH:mm:ss[+-]OO:oo)
 * Format res as number followed by unit (ms, s, m, h, d)
 * Pass signal names (regex) in `name` parameter.
 * E.g. `/signal/{start}/{end}/{res}?name=BMS_TractiveSystemVoltage`
 */
#[deprecated(note = "Use signal_tiles instead")]
 async fn signal_time_range(
    Path((start, end, res)): Path<(String, String, String)>, 
    Query((name,)): Query<(Option<String>,)>, 
    State(state): State<AppState>
) -> impl IntoResponse {
    // check RFC3339 format
    let (start_utc, end_utc) = 
        if let (Some(s), Some(e)) = 
            (rfc3339_to_utc_str(&start), rfc3339_to_utc_str(&end)) 
        {
            (s, e)
        } else {
            return (StatusCode::BAD_REQUEST, "Bad date format, should be RFC3339 format".to_string());
        };

    let res_re = Regex::new(r"^\d+(ms|s|m|h|d)$").unwrap();
    if !res_re.is_match(&res) {
        return (StatusCode::BAD_REQUEST, "Bad resolution format, should be a number followed by unit".to_string());
    }

    let mut date_query = format!(r#"
    from(bucket: "{}")
    |> range(start: {start_utc}, stop: {end_utc})
    |> aggregateWindow(every: {res}, fn: mean, createEmpty: false)
    |> filter(fn: (r) => r["_measurement"] == "{}")
    |> filter(fn: (r) => r["source"] == "radio")
    "#, &CONFIG.influxdb_bucket, &CONFIG.influxdb_measurement);
    
    if let Some(signal) = name {
        let _ = match Regex::new(&signal) {
            Ok(_) => {},
            Err(_) => {
                return (StatusCode::BAD_REQUEST, "Bad name regex format".to_string());
            }
        };
        date_query.push_str(&format!(
            r#"|> filter(fn: (r) => r["signal_name"] =~ /^{}/)"#,
            signal
        ));
    }

    let req: Result<Vec<_>, influxdb2::RequestError> = select! {
        val = state.influx_client.query::<InfluxSignalRow>(
            Some(influxdb2::models::Query::new(date_query))
        ) => val,
        _ = sleep(Duration::from_millis(INFLUX_QUERY_TIMEOUT_MS)) => {
            return (StatusCode::REQUEST_TIMEOUT, "InfluxDB query timed out, try smaller query!".to_string());
        }
    };
        
    match req {
        Ok(res) => {
            return (StatusCode::OK, serde_json::to_string(&res).unwrap());
        },
        Err(e) => {
            return (StatusCode::BAD_REQUEST, format!("{}", e.to_string()));
        }
    }
}


/**
 * Gets signal values, timestamp, and name.
 * Format date as RFC3339 (i.e. YYYY-MM-DDTHH:mm:ssZ or YYYY-MM-DDTHH:mm:ss[+-]OO:oo)
 * E.g. `/signal/tiles/BMS_TractiveSystemVoltage/2026-04-02T00:00:00Z/2026-04-02T01:00:00Z`
 * By default, fetches from radio source, but specify source in query parameter (e.g. `?source=sd_card`) to fetch from sd card source or `radio` source explicitly
 */
async fn signal_tiles(
    Path((signal, start, end)): Path<(String, String, String)>, 
    Query((source,)): Query<(Option<String>,)>,
    State(state): State<AppState>
) -> impl IntoResponse {
    // todo optionally check if signal name is valid
    // todo actually cache the tiles
    let start_time = SystemTime::now();
    let (start_utc, end_utc) = 
        if let (Some(s), Some(e)) = 
        (rfc3339_to_utc(&start), rfc3339_to_utc(&end)) {
            (s, e)
        } else {
            return (StatusCode::BAD_REQUEST, "Bad date format, should be RFC3339 format".to_string());
        };
    
    let source_enum = match source {
        Some(s) => {
            if let Ok(e) = from_str(&s) {
                e
            } else {
                return (StatusCode::BAD_REQUEST, "Bad source!".to_string());
            }
        }
        _ => InfluxSignalSource::Radio,
    };
    
    match get_signals(state.influx_client, source_enum, state.signal_tile_cache.clone(), signal, start_utc, end_utc).await {
        Ok(res) => {
            dprintln!("Querying signals took {}ms", start_time.elapsed().unwrap().as_millis());
            let total_size: usize = state.signal_tile_cache.iter().map(|(key, value)| {
                mem::size_of_val(&key) + mem::size_of_val(&value)
            }).sum();
            dprintln!("Current cache size in bytes: {}", total_size);
            return (StatusCode::OK, serde_json::to_string(&res).unwrap());
        },
        Err(e) => {
            return e;
        }
    };
}

async fn signal_csv() -> impl IntoResponse {
    // TODO implement
    // todo!("to implement signal csv download");
    return (StatusCode::OK, Json(()));
}

pub fn get_signal_router() -> Router<AppState> {
    return Router::new()
        .route("/signal/nodes", get(nodes))
        .route("/signal/metadata", get(metadata))
        .route("/signal/{start}/{end}/{res}", get(signal_time_range))
        .route("/signal/csv", get(signal_csv))
        .route("/signal/tiles/{signal}/{start}/{end}", get(signal_tiles));
}