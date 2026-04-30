pub mod subtable_clients;
pub mod subtable_api_handler;
pub mod signal_api_handler;
pub mod signal_tile;
pub mod sd_api_handler;
pub mod sd_utils;
pub mod transmit_api_handler;


use std::sync::Arc;
use tokio::sync::{RwLock, broadcast};
use jsoncan_rust::can_database::CanDatabase;
use crate::tasks::{client_api::{signal_tile::SignalTileCache, subtable_clients::Clients}, telem_message::TelemetryOutgoingMessage};

#[derive(Clone)]
pub struct AppState {
    pub can_db: Arc<CanDatabase>,
    pub clients: Arc<RwLock<Clients>>,
    pub influx_client: Arc<influxdb2::Client>,
    pub client_out_msg_tx: broadcast::Sender<TelemetryOutgoingMessage>,

    pub signal_tile_cache: SignalTileCache,
}
