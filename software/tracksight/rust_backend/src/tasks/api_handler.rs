use std::sync::Arc;
use axum::Router;
use moka::future::Cache;
use tokio::select;
use tokio::sync::RwLock;
use tokio::net::TcpListener;
use socketioxide::{SocketIo, extract::SocketRef};
use jsoncan_rust::can_database::CanDatabase;
use tower_http::cors::{CorsLayer, Any};
use mdns_sd::{ServiceDaemon, ServiceInfo};

use crate::tasks::client_api::signal_tile::LRU_CACHE_CAPACITY;
#[allow(unused_imports)]
use crate::utils::yellow;
use crate::config::CONFIG;
use crate::tasks::{HealthCheckSender, HealthCheckSenderExt, ResultExt, ShutdownReceiver, Task};
use crate::tasks::client_api::AppState;
use crate::tasks::client_api::clients::Clients;
use crate::tasks::client_api::signal_api_handler::get_signal_router;
use crate::tasks::client_api::subtable_api_handler::get_subtable_router;
use crate::vprintln;

pub async fn run_api_handler(
    mut shutdown_rx: ShutdownReceiver, 
    health_check_tx: HealthCheckSender, 
    clients: Arc<RwLock<Clients>>, 
    can_db: Arc<CanDatabase>
) {
    vprintln!("{}", yellow("API handler task started."));
    
    // Axum
    let addr = format!("0.0.0.0:{}", CONFIG.backend_port);
    let listener = TcpListener::bind(addr).await
        .unwrap_or_fail_health_check(&health_check_tx, Task::ApiHandler).await;

    let mdns = ServiceDaemon::new()
        .unwrap_or_fail_health_check(&health_check_tx, Task::ApiHandler).await;

    let service_type = "_http._tcp.local.";
    let instance_name = "server";

    let service_info = ServiceInfo::new(
        service_type,
        instance_name,
        &format!("{}.local.", instance_name),
        &CONFIG.mdns_local_ip,
        CONFIG.backend_port,
        None,
    ).unwrap_or_fail_health_check(&health_check_tx, Task::ApiHandler).await;

    mdns.register(service_info)
        .unwrap_or_fail_health_check(&health_check_tx, Task::ApiHandler).await;
    // running at this location
    println!("Server hosted on {} running at http://{}.local:{}", CONFIG.mdns_local_ip, instance_name, CONFIG.backend_port);

    // Websocket
    let (socket_layer, io) = SocketIo::new_layer();

    let app_state = AppState {
        can_db: can_db,
        clients: clients.clone(),
        influx_client: Arc::new(influxdb2::Client::new(
            &CONFIG.influxdb_url,
            &CONFIG.influxdb_org,
            &CONFIG.influxdb_token
        )),

        signal_tile_cache: Cache::builder()
            .max_capacity(LRU_CACHE_CAPACITY) // max number of tiles in cache, adjust as needed
            .build(),
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

    // set up complete, send health_check
    health_check_tx.send_health_check(Task::ApiHandler, true).await;

    // this is so quirky it's amazing
    // the select macro waits for one of these to finish
    // if shutdown finishes first, leave select block
    select! {
        _ = shutdown_rx.recv() => {
            vprintln!("API handler task shutting down.");
        },
        _ = axum::serve(listener, app) => {
            println!("Error occurred");
        }
    }
    vprintln!("{}", yellow("API handler task ended."));
}
