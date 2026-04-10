use axum::{Router, http::StatusCode, response::IntoResponse, routing::get};

use crate::tasks::client_api::AppState;

async fn sd_list() -> impl IntoResponse {
    // todo find all detachable devices, if parseable by logfs, list file names

    let file_names: Vec<String> = Vec::new();

    return (StatusCode::OK, serde_json::to_string(&file_names).unwrap());
}

pub fn get_sd_router() -> Router<AppState> {
    return Router::new()
        .route("/sd/list", get(sd_list));
}