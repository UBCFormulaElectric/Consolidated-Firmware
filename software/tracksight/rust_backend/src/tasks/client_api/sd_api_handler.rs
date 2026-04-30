use axum::{Json, Router, http::StatusCode, response::IntoResponse, routing::{get, post}};
use serde::Deserialize;

use crate::tasks::client_api::{AppState, sd_utils::{find_detachable_drives, get_logfs, ls_deep}};

/**
 * Mainly supported for Ubuntu, not guaranteed on anything else
 */
async fn sd_list() -> impl IntoResponse {
    return (StatusCode::OK, serde_json::to_string(&find_detachable_drives()).unwrap());
}

#[derive(Deserialize)]
struct SdShowPayload {
    drive: String,
}

/**
 * Show nested directory structure of the drive
 */
async fn sd_show(Json(SdShowPayload{drive}): Json<SdShowPayload>) -> impl IntoResponse {
    let logfs = match get_logfs(drive.clone()) {
        Ok(logfs) => logfs,
        Err(_) => return (StatusCode::BAD_REQUEST, format!("Failed to mount {drive}")),
    };
    match ls_deep(logfs) {
        Ok(files) => return (StatusCode::OK, serde_json::to_string(&files).unwrap()),
        Err(_) => return (StatusCode::BAD_REQUEST, format!("Failed to read drive {drive}")),
    }
}

#[derive(Deserialize)]
struct SdDumpPayload {
    drive: String,
    file: String,
}

async fn sd_dump(Json(SdDumpPayload{drive, file}): Json<SdDumpPayload>) -> impl IntoResponse {
    return (StatusCode::OK, format!("Dump {file} from drive {drive}"));
}

pub fn get_sd_router() -> Router<AppState> {
    return Router::new()
        .route("/sd/list", get(sd_list))
        .route("/sd/show", get(sd_show))
        .route("/sd/dump", post(sd_dump));
}