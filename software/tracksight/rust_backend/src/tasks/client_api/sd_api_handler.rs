use axum::{Json, Router, extract::{Query, State}, http::StatusCode, response::IntoResponse, routing::{get, post}};
use serde::Deserialize;

use crate::tasks::client_api::{AppState, sd_utils::{dump_sd_file, find_detachable_drives, get_logfs, ls_deep}};

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
 * Pass drive in `drive` parameter, e.g. `/sd/show?drive=/dev/sdb`
 */
async fn sd_show(Query(SdShowPayload{drive}): Query<SdShowPayload>) -> impl IntoResponse {
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
    overwrite: Option<bool>,
}

/**
 * Parses file, reads the entire file, parses, then pushes to database as separate category for SD card.
 * If file already exists in database, overwrite if `overwrite` is true, otherwise returns code 409.
 */
async fn sd_dump(State(state): State<AppState>, Json(SdDumpPayload{drive, file, overwrite}): Json<SdDumpPayload>) -> impl IntoResponse {
    // TODO some way to check database if file already dumped
    // TODO mutex, make sure the same file is not dumped multiple times concurrently
    if dump_sd_file(state.can_db, state.influx_client, &drive, &file).await.is_err() {
        return (StatusCode::BAD_REQUEST, format!("Failed to read file {file} from drive {drive}"));
    }
    
    return (StatusCode::OK, format!("Dump {file} from drive {drive}"));
}

pub fn get_sd_router() -> Router<AppState> {
    return Router::new()
        .route("/sd/list", get(sd_list))
        .route("/sd/show", get(sd_show))
        .route("/sd/dump", post(sd_dump));
}


// TODO remove mock after done


async fn sd_list_mock() -> impl IntoResponse {
    return (StatusCode::OK, serde_json::to_string(&vec!["/dev/sdb", "/dev/sdc"]).unwrap());
}

async fn sd_show_mock(Query(SdShowPayload{drive}): Query<SdShowPayload>) -> impl IntoResponse {
    let files = match drive.as_str() {
        "/dev/sdb" => vec![
            "/file1.txt".to_string(),
            "/dir1/file2.txt".to_string(),
            "/dir1/file3.txt".to_string(),
            "/dir2/file4.txt".to_string(),
        ],
        "/dev/sdc" => vec![
            "/file5.txt".to_string(),
            "/dir3/file6.txt".to_string(),
            "/dir3/file7.txt".to_string(),
            "/dir4/file8.txt".to_string(),
        ],
        _ => return (StatusCode::BAD_REQUEST, format!("Drive {drive} not found")),
    };
    return (StatusCode::OK, serde_json::to_string(&files).unwrap());
}

async fn sd_dump_mock(Json(SdDumpPayload{drive, file, overwrite}): Json<SdDumpPayload>) -> impl IntoResponse {
    match drive.as_str() {
        "/dev/sdb" => {
            match file.as_str() {
                "/file1.txt" => (),
                "/dir1/file2.txt" => (),
                "/dir1/file3.txt" => (),
                "/dir2/file4.txt" => (),
                _ => return (StatusCode::BAD_REQUEST, format!("File {file} not found on drive {drive}")),
            }
        },
        "/dev/sdc" => {
            if overwrite == Some(true) {
                match file.as_str() {
                    "/file5.txt" => (),
                    "/dir3/file6.txt" => (),
                    "/dir3/file7.txt" => (),
                    "/dir4/file8.txt" => (),
                    _ => return (StatusCode::BAD_REQUEST, format!("File {file} not found on drive {drive}")),
                }
            }
            return (StatusCode::CONFLICT, format!("File {file} already exists on drive {drive}, overwrite by passing overwrite=true in the request body."));
        },
        _ => return (StatusCode::BAD_REQUEST, format!("Drive {drive} not found")),
    };
    return (StatusCode::OK, format!("Dump {file} from drive {drive}"));
}

pub fn get_sd_router_mock() -> Router<AppState> {
    return Router::new()
        .route("/sd/list", get(sd_list_mock))
        .route("/sd/show", get(sd_show_mock))
        .route("/sd/dump", post(sd_dump_mock));
}