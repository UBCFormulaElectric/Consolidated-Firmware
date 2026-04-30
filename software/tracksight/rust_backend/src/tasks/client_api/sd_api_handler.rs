use std::fs;

use axum::{Json, Router, extract::Path, http::StatusCode, response::IntoResponse, routing::{get, post}};
use serde::Deserialize;

use crate::tasks::client_api::AppState;

/**
 * Mainly supported for Ubuntu, not guaranteed on anything else
 */
async fn sd_list() -> impl IntoResponse {
    let mut drives = Vec::new();
    let sys_block_path = "/sys/block";

    if let Ok(entries) = fs::read_dir(sys_block_path) {
        for entry in entries.flatten() {
            let path = entry.path();
            // Check if it's a removable device
            let removable_path = path.join("removable");
            if let Ok(content) = fs::read_to_string(&removable_path) {
                if content.trim() == "1" {
                    if let Some(name) = path.file_name() {
                        if let Some(name_str) = name.to_str() {
                            drives.push(format!("/dev/{}", name_str));
                        }
                    }
                }
            }
        }
    }
    return (StatusCode::OK, serde_json::to_string(&drives).unwrap());
}

/**
 * Show nested directory structure of the drive
 */
async fn sd_show(Path(drive): Path<String>) -> impl IntoResponse {
    return (StatusCode::OK, format!("Show files from drive {drive}"));
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
        .route("/sd/show/:drive", get(sd_show))
        .route("/sd/dump", post(sd_dump));
}