use axum::Json;
use axum::response::IntoResponse;
use axum::routing::post;
use axum::{Router, routing::get};


struct SubscribePayload {
    signal_name: String,
    client_id: String,
}


async fn subscribe(Json(payload): Json<SubscribePayload>) -> impl IntoResponse {
    
    return (axum::http::StatusCode::OK, "");
}

pub fn get_subtable_router() -> Router {
    return Router::new()
        .route("/subscribe", post(subscribe));
}
