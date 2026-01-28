
use std::sync::Arc;
use axum::response::IntoResponse;
use axum::serve::Listener;
use jsoncan_rust::can_database::CanDatabase;
use jsoncan_rust::parsing::JsonCanParser;
use tokio::select;
use tokio::sync::{RwLock, broadcast};
use tokio::net::TcpListener;
use axum::{Router, routing::get};
use axum::extract::ws::{WebSocket, WebSocketUpgrade};
use socketioxide::{SocketIo, extract::SocketRef};
use crate::config::CONFIG;
use crate::tasks::client_api::clients::Clients;
use crate::tasks::client_api::subtable::get_subtable_router;

#[derive(Clone)]
struct AppState {
    candb: CanDatabase,
    clients: Arc<RwLock<Clients>>,
}

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

    // TODO dupe logic
    let parser = JsonCanParser::new(CONFIG.jsoncan_config_path.clone());
    let can_db = match CanDatabase::from(parser) {
        Ok(db) => db,
        Err(e) => {
            eprintln!("Failed to load CAN database: {:?}", e);
            return;
        }
    };
    
    let app_state = AppState {
        candb: can_db,
        clients: clients.clone(),
    };

    let app = Router::new()
        .with_state(app_state)
        .layer(socket_layer)
        .merge(get_subtable_router())
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
