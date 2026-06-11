use axum::{Json, Router, http::StatusCode, response::IntoResponse, routing::get};
use crate::tasks::{client_api::AppState, get_backend_telem_state};


async fn backend_telem_state() -> impl IntoResponse {
    let backend_telem_state = get_backend_telem_state().read().await.clone();
    return (StatusCode::OK, Json(backend_telem_state));
}

pub fn get_misc_router() -> Router<AppState> {
    return Router::new()
        .route("/misc/backendtelemstate", get(backend_telem_state));
}
