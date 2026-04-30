

/*
    For sending messages to DAM directly from frontend
*/


use axum::{Router, extract::State, http::StatusCode, response::IntoResponse, routing::get};

use crate::tasks::{client_api::AppState, telem_message::TelemetryOutgoingMessage};

/**
 * Manually trigger an NTP sync on the DAM
 */
async fn ntp_trigger(State(state): State<AppState>) -> impl IntoResponse {
    if state.client_out_msg_tx.send(TelemetryOutgoingMessage::NtpTrigger).is_ok() {
        return StatusCode::OK; // TODO probably make this return code more meaningful, e.g. verify that DAM has properly synched ntp and packet wasn't lost on the way
    } else {
        return StatusCode::INTERNAL_SERVER_ERROR;
    }
}

pub fn get_transmit_router() -> Router<AppState> {
    return Router::new()
        .route("/transmit/ntp", get(ntp_trigger));
}