use std::collections::HashMap;

use axum::{Json, Router, extract::{Query, State}, http::StatusCode, response::IntoResponse, routing::get};
use jsoncan_rust::can_database::CanMessage;
use serde::{Deserialize, Serialize};
use regex::Regex;

use crate::tasks::client_api::AppState;

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
struct MetadataParam {
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
 * E.g. `/signal?name=INVFR_bError,*_bReserve`
 */
async fn metadata(Query(mut param): Query<MetadataParam>, State(state): State<AppState>) -> impl IntoResponse {
    // todo unwrap hehe
    let regex = Regex::new(param.name.get_or_insert("*".to_string())).unwrap();

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
    todo!("query from influxdb");
    return (StatusCode::OK, Json(()));
}

async fn signal_csv() -> impl IntoResponse {
    // TODO implement
    todo!("to implement signal csv download");
    return (StatusCode::OK, Json(()));
}

pub fn get_signal_router() -> Router<AppState> {
    return Router::new()
        .route("/signal/nodes", get(nodes))
        .route("/signal/metadata", get(metadata))
        .route("/signal", get(signal))
        .route("/signal/csv", get(signal_csv));
}