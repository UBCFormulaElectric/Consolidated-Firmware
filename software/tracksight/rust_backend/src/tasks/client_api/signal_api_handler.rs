use std::collections::HashMap;

use axum::{Json, Router, extract::{Path, Query, State}, http::StatusCode, response::IntoResponse, routing::get};
use influxdb2::{FromDataPoint};
use jsoncan_rust::can_database::CanMessage;
use serde::{Deserialize, Serialize};
use regex::Regex;
use chrono::{DateTime, FixedOffset};

use crate::{config::CONFIG, tasks::client_api::AppState};

/**
 * Gets the list of all nodes (str) in the current parser.
 */
async fn nodes(State(state): State<AppState>) -> impl IntoResponse {
    let nodes = state.can_db.nodes
        .iter().map(|node| node.name.clone())
        .collect::<Vec<_>>();

    return (StatusCode::OK, Json(nodes));
}

#[derive(Debug, Deserialize)]
struct SignalNameParam {
    name: Option<String>,
}

#[derive(Debug, Serialize)]
struct SignalMetadata {
    name: String,
    min_val: f64,
    max_val: f64,
    unit: Option<String>,
    enum_type: Option<String>,
    tx_node: String,
    cycle_time_ms: Option<u32>,
    id: u32,
    msg_name: String,
}

/**
 * Gets metadata of the signal of the current parser.
 * Pass signal names (regex) in `name` parameter.
 * E.g. `/signal/metadata?name=INVFR_bError`
 */
async fn metadata(Query(mut param): Query<SignalNameParam>, State(state): State<AppState>) -> impl IntoResponse {
    let regex = match Regex::new(param.name.get_or_insert(".*".to_string())) {
        Ok(regex) => regex,
        Err(_) => {
            return (StatusCode::BAD_REQUEST, Json::default());
        }
    };

    let flat_map = | msg: &CanMessage | {
        return msg.signals.iter().map(
            | signal | {
                (signal.name.clone(),
                SignalMetadata {
                    name: signal.name.clone(),
                    min_val: signal.min,
                    max_val: signal.max,
                    unit: signal.unit.clone(),
                    enum_type: signal.enum_name.clone(),
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

async fn signal() -> impl IntoResponse {
    // TODO implement
    // todo!("query from influxdb");
    return (StatusCode::OK, serde_json::to_string(&Vec::<String>::new()).unwrap());
}

#[derive(Debug, Serialize, FromDataPoint, Default)]
struct InfluxRow {
    // rename influx field names to match with frontend names
    #[serde(rename = "timestamp")]
    time: DateTime<FixedOffset>,
    value: f64,
    measurement: String,
    #[serde(rename = "name")]
    signal_name: String,
}

/**
 * Gets signal values, timestamp, and name.
 * Format date as YYYY-MM-DD
 * Pass signal names (regex) in `name` parameter.
 * E.g. `/signal/2026-03-01?name=BMS_TractiveSystemVoltage`
 */
async fn signal_date(Path(date): Path<String>, Query(param): Query<SignalNameParam>, State(state): State<AppState>) -> impl IntoResponse {
    let re = Regex::new(r"^\d{4}-\d{2}-\d{2}$").unwrap();
    if !re.is_match(&date) {
        return (StatusCode::BAD_REQUEST, "Bad date format, should be YYYY-MM-DD".to_string());
    }
    
    let mut date_query = format!(r#"
    from(bucket: "{}")
    |> range(start: {date}T00:00:00Z, stop: {date}T23:59:59Z)
    |> filter(fn: (r) => r["_measurement"] == "{}")
    "#, &CONFIG.influxdb_bucket, &CONFIG.influxdb_measurement);
    
    if let Some(signal) = param.name {
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

    let req: Result<Vec<_>, influxdb2::RequestError> = 
        state.influx_client.query::<InfluxRow>(
            Some(influxdb2::models::Query::new(date_query))
        ).await;

    match req {
        Ok(res) => {
            return (StatusCode::OK, serde_json::to_string(&res).unwrap());
        },
        Err(e) => {
            return (StatusCode::BAD_REQUEST, format!("{e:?}"));
        }
    }

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
        .route("/signal", get(signal))
        .route("/signal/{date}", get(signal_date))
        .route("/signal/csv", get(signal_csv));
}