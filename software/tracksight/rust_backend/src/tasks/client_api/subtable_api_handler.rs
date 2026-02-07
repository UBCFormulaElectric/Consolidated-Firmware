use axum::Json;
use axum::extract::{Path, State};
use axum::http::StatusCode;
use axum::response::IntoResponse;
use axum::routing::post;
use axum::{Router, routing::get};
use serde::{Deserialize, Serialize};

use crate::tasks::client_api::AppState;

/*
    Subscription table for tracking users subscribed to which signal
*/


#[derive(Debug, Deserialize)]
struct SubscribePayload {
    signal: String,
    sid: String,
}

#[derive(Debug, Serialize)]
struct SubscribeResponse {
    msg: Option<String>,
    error: Option<String>,
}

async fn subscribe(State(state): State<AppState>, Json(payload): Json<SubscribePayload>) -> impl IntoResponse {
    println!("Subscribing client {} to signal {}", payload.sid, payload.signal);

    if !state.can_db.is_signal_valid(&payload.signal) {
        eprintln!("Client {} failed to subscribe to invalid signal {}", payload.sid, payload.signal);
        return (StatusCode::BAD_REQUEST, Json(SubscribeResponse {
            msg: None,
            error: Some(format!("Signal {} does not exist", payload.signal)),
        }));
    }

    state.clients.write().await.subscribe_client_to_signal(&payload.sid, &payload.signal);

    return (StatusCode::OK, Json(SubscribeResponse {
        msg: Some(format!("Subscribed {} to signal {}", payload.sid, payload.signal)),
        error: None,
    }));
}

async fn unsubscribe(State(state): State<AppState>, Json(payload): Json<SubscribePayload>) -> impl IntoResponse {
    println!("Unsubscribing client {} from signal {}", payload.sid, payload.signal);

    state.clients.write().await.unsubscribe_client_from_signal(&payload.sid, &payload.signal);

    return (StatusCode::OK, Json(SubscribeResponse {
        msg: Some(format!("Unsubscribed {} from signal {}", payload.sid, payload.signal)),
        error: None,
    }));
}

async fn client_signals(Path(client_id): Path<String>, State(state): State<AppState>) -> impl IntoResponse {
    let signals = state.clients.read().await.get_signals_of_client(&client_id);

    return (StatusCode::OK, Json(signals));
}

pub fn get_subtable_router() -> Router<AppState> {
    return Router::new()
        .route("/subscribe", post(subscribe))
        .route("/unsubscribe", post(unsubscribe))
        .route("/{sid}/signals", get(client_signals));
}
