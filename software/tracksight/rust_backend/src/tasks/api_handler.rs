
use std::sync::Arc;
use axum::response::IntoResponse;
use axum::serve::Listener;
use tokio::select;
use tokio::sync::{RwLock, broadcast};
use tokio::net::TcpListener;
use axum::{Router, routing::get};
use axum::extract::ws::{WebSocket, WebSocketUpgrade};
use socketioxide::{SocketIo, extract::SocketRef};
use crate::config::CONFIG;
use crate::tasks::client_api::clients::Clients;



pub async fn run_api_handler(mut shutdown_rx: broadcast::Receiver<()>, clients: Arc<RwLock<Clients>>) {
    let addr = format!("0.0.0.0:{}", CONFIG.backend_port);
    let listener = TcpListener::bind(addr).await.unwrap();

    let (socket_layer, io) = SocketIo::new_layer();

    // default socketio endpoint
    io.ns("/", |socket: SocketRef| async move {
        let client_id = socket.id.to_string();
        println!("{} connected", client_id);

        clients.write().await.add_client(&client_id, &socket);

        socket.on_disconnect(async move || {
            clients.write().await.remove_client(&client_id);
            println!("{} disconnected", client_id);
        });

    });

    let app: Router<> = Router::new()
        .layer(socket_layer);

    select! {
        _ = shutdown_rx.recv() => {
            println!("Shutting down API handler task.");
        },
        _ = axum::serve(listener, app.into_make_service()) => {
            println!("Error occurred");
        }
    }
}
