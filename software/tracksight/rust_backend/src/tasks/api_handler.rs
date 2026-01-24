use axum::response::IntoResponse;
use axum::serve::Listener;
use tokio::select;
use tokio::sync::broadcast;
use tokio::net::TcpListener;
use tokio_tungstenite::accept_async;
use axum::{Router, routing::get};
use axum::extract::ws::{WebSocket, WebSocketUpgrade};
use crate::config::CONFIG;



pub async fn run_api_handler(mut shutdown_rx: broadcast::Receiver<()>) {
    let addr = format!("0.0.0.0:{}", CONFIG.backend_port);
    let listener = TcpListener::bind(addr).await.unwrap();

    let app: Router<> = Router::new()
        .route("/ws", get(handle_connection));

    select! {
        _ = shutdown_rx.recv() => {
            println!("Shutting down API handler task.");
        },
        _ = axum::serve(listener, app.into_make_service()) => {
            println!("Error occurred");
        }
    }
}

async fn handle_connection(ws: WebSocketUpgrade) -> impl IntoResponse {
    let _ = ws.on_upgrade(async |mut socket: WebSocket| {
        println!("client connected");

        while let Some(Ok(_)) = socket.recv().await {
            // ignore
        }

        println!("client disconnected")
    });
}