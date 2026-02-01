use axum::Json;
use axum::http::StatusCode;
use axum::response::IntoResponse;
use axum::routing::post;
use axum::{Router, routing::get};
use serde::{Deserialize, Serialize};


#[derive(Debug, Deserialize)]
struct SubscribePayload {
    signal_name: String,
    client_id: String,
}

async fn subscribe(Json(payload): Json<SubscribePayload>) -> impl IntoResponse {
    println!("Subscribing client {} to signal {}", payload.client_id, payload.signal_name);
    (StatusCode::OK, "")
}

pub fn get_subtable_router() -> Router {
    return Router::new()
        .route("/subscribe", post(subscribe));
}
