use std::sync::Arc;
use axum::Router;
use colored::Colorize;
use tokio::select;
use tokio::sync::{RwLock, broadcast};
use tokio::net::TcpListener;
use socketioxide::{SocketIo, extract::SocketRef};
use jsoncan_rust::can_database::CanDatabase;
use tower_http::cors::{CorsLayer, Any};
use mdns_sd::{ServiceDaemon, ServiceInfo};

use crate::config::CONFIG;
use crate::tasks::client_api::AppState;
use crate::tasks::client_api::clients::Clients;
use crate::tasks::client_api::signal_api_handler::get_signal_router;
use crate::tasks::client_api::subtable_api_handler::get_subtable_router;

pub async fn run_api_handler(mut shutdown_rx: broadcast::Receiver<()>, clients: Arc<RwLock<Clients>>, can_db: Arc<CanDatabase>) {
    println!("{}", "API handler task started.".yellow());
    
    // Axum
    let addr = format!("0.0.0.0:{}", CONFIG.backend_port);
    let listener = TcpListener::bind(addr).await.unwrap();

    let mdns = ServiceDaemon::new().unwrap();

    let service_type = "_http._tcp.local.";
    let instance_name = "server";

    let service_info = ServiceInfo::new(
        service_type,
        instance_name,
        &format!("{}.local.", instance_name),
        &CONFIG.mdns_local_ip,
        CONFIG.backend_port,
        None,
    ).unwrap();

    mdns.register(service_info).unwrap();
    // running at this location
    println!("Server hosted on {} running at http://{}.local:{}", CONFIG.mdns_local_ip, instance_name, CONFIG.backend_port);

    // Websocket
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

    // this is so quirky it's amazing
    // the select macro waits for one of these to finish
    // if shutdown finishes first, leave select block
    select! {
        _ = shutdown_rx.recv() => {
            println!("API handler task shutting down.");
        },
        _ = axum::serve(listener, app) => {
            println!("Error occurred");
        }
    }
    println!("{}", "API handler task ended.".yellow());
}
