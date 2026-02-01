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
    unit: String,
    // todo enum
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

    let filter_map = | msg: &CanMessage | -> Option<(String, SignalMetadata)> {
        if !regex.is_match(&msg.name) {
            return None;
        }

        Some((msg.name.clone(), SignalMetadata {
            //todo
        }))
    };

    let metadatas: HashMap<String, SignalMetadata> = state.can_db.get_all_msgs()
        .unwrap_or_default()
        .iter()
        .filter_map(filter_map)
        .collect();

    return (StatusCode::OK, Json(metadatas));
}

async fn signal() -> impl IntoResponse {
    // TODO implement
    return (StatusCode::OK, Json(()));
}


pub fn get_subtable_router(app_state: AppState) -> Router {
    return Router::new()
        .route("/signal/nodes", get(nodes))
        .route("/signal/metadata", get(metadata))
        .route("/signal", get(signal))
        .with_state(app_state);
}