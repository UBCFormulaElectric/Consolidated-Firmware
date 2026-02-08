use std::sync::Arc;
use axum::Router;
use tokio::select;
use tokio::sync::{RwLock, broadcast};
use tokio::net::TcpListener;
use socketioxide::{SocketIo, extract::SocketRef};
use jsoncan_rust::can_database::CanDatabase;
use tower_http::cors::{CorsLayer, Any};

use crate::config::CONFIG;
use crate::tasks::client_api::AppState;
use crate::tasks::client_api::clients::Clients;
use crate::tasks::client_api::signal_api_handler::get_signal_router;
use crate::tasks::client_api::subtable_api_handler::get_subtable_router;

pub async fn run_api_handler(mut shutdown_rx: broadcast::Receiver<()>, clients: Arc<RwLock<Clients>>, can_db: Arc<CanDatabase>) {
    let addr = format!("0.0.0.0:{}", CONFIG.backend_port);
    let listener = TcpListener::bind(addr).await.unwrap();

    let (socket_layer, io) = SocketIo::new_layer();

    let app_state = AppState {
        can_db,
        clients: clients.clone(),
    };

    let cors = CorsLayer::new()
        .allow_origin(Any)
        .allow_methods(Any)
        .allow_headers(Any);

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

    let app = Router::new()
        .layer(socket_layer)
        .nest("/api/v1/", get_subtable_router())
        .nest("/api/v1/", get_signal_router())
        .with_state(app_state)
        .layer(cors)
        .into_make_service();

    select! {
        _ = shutdown_rx.recv() => {
            println!("Shutting down API handler task.");
        },
        _ = axum::serve(listener, app) => {
            println!("Error occurred");
        }
    }
}
