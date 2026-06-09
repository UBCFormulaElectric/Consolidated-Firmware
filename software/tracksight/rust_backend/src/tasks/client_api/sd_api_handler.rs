use axum::{Json, Router, extract::{Query, State}, http::StatusCode, response::IntoResponse, routing::{get, post}};
use futures::stream;
use influxdb2::{FromDataPoint, models::DataPoint};
use serde::Deserialize;

use crate::{config::CONFIG, tasks::client_api::{AppState, sd_utils::{dump_sd_file, find_detachable_drives, format_drive, get_logfs, ls_deep, SdFormatError}}};

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

#[derive(Debug, FromDataPoint, Default)]
struct SdCardDumpRow {
    car: String,
    file_name: String,
    dumped: bool,
}

/**
 * Parses file, reads the entire file, parses, then pushes to database as separate category for SD card.
 * If file already exists in database, overwrite if `overwrite` is true, otherwise returns code 409.
 */
async fn sd_dump(State(state): State<AppState>, Json(SdDumpPayload{drive, file, overwrite}): Json<SdDumpPayload>) -> impl IntoResponse {
    // TODO some way to check database if file already dumped
    // TODO mutex, make sure the same file is not dumped multiple times concurrently
    let query = format!(r#"
        from(bucket: "{}")
        |> range(start: 0)
        |> filter(fn: (r) => r["_measurement"] == "sd_dumps")
        |> filter(fn: (r) => r.car == "{}")
        |> filter(fn: (r) => r.file_name == "{}")
        |> limit(n: 1)
    "#, &CONFIG.influxdb_bucket, &CONFIG.influxdb_measurement, &file);

    let dumped_res = state.influx_client.query::<SdCardDumpRow>(Some(influxdb2::models::Query::new(query))).await;

    match dumped_res {
        Ok(vec) => {
            if vec.iter().find(|row| row.dumped).is_some() {
                return (StatusCode::CONFLICT, format!("File {file} already exists in database (overwrite not implemented)."));
            }
        },
        Err(_) => return (StatusCode::INTERNAL_SERVER_ERROR, format!("Failed to query database for existing dumps of file")),
    };
    
    match dump_sd_file(
        state.can_db, 
        state.influx_client.clone(), 
        &drive, 
        &file
    ).await {
        Ok(_) => {
            // TODO this is not a good way to define it ngl but itll do
            let dump_record = DataPoint::builder("sd_dumps")
                .tag("car", &CONFIG.influxdb_measurement)
                .tag("file_name", &file)
                .field("dumped", true)
                .build().unwrap();
            let _ = state.influx_client.write(&CONFIG.influxdb_bucket, stream::iter([dump_record])).await;
        },
        Err(e) => return (StatusCode::BAD_REQUEST, format!("Failed to read file {file} from drive {drive}: {e:?}")),
    }
    
    return (StatusCode::OK, format!("Dump {file} from drive {drive}"));
}

#[derive(Debug, Deserialize)]
struct SdFormatPayload {
    drive: String,
}

async fn sd_format(Json(SdFormatPayload{drive}): Json<SdFormatPayload>) -> impl IntoResponse {
    match format_drive(&drive) {
        Ok(_) => (StatusCode::OK, format!("Formatted drive {drive}")),
        Err(SdFormatError::NotDetachable) =>
            (StatusCode::BAD_REQUEST, format!("Drive {drive} is not a removable drive")),
        Err(SdFormatError::DiskOpenFailed) =>
            (StatusCode::INTERNAL_SERVER_ERROR, format!("Failed to open drive {drive}")),
        Err(SdFormatError::FormatFailed(e)) =>
            (StatusCode::INTERNAL_SERVER_ERROR, format!("Failed to format drive {drive}: logfs error {e}")),
    }
}

pub fn get_sd_router() -> Router<AppState> {
    return Router::new()
        .route("/sd/list", get(sd_list))
        .route("/sd/show", get(sd_show))
        .route("/sd/dump", post(sd_dump))
        .route("/sd/format", post(sd_format));
}
