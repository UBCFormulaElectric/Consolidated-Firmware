pub mod sd_api_handler;
pub mod sd_utils;
pub mod signal_api_handler;
pub mod signal_tile;
pub mod subtable_api_handler;
pub mod subtable_clients;
pub mod transmit_api_handler;

use crate::tasks::{
    client_api::{signal_tile::SignalTileCache, subtable_clients::Clients},
    telem_message::TelemetryOutgoingMessage,
};
use jsoncan_rust::can_database::CanDatabase;
use std::sync::Arc;
use tokio::sync::{broadcast, RwLock};

#[derive(Clone)]
pub struct AppState {
    pub can_db: Arc<CanDatabase>,
    pub clients: Arc<RwLock<Clients>>,
    pub influx_client: Arc<influxdb2::Client>,
    pub client_out_msg_tx: broadcast::Sender<TelemetryOutgoingMessage>,

    pub signal_tile_cache: SignalTileCache,
}

pub const INFLUX_QUERY_TIMEOUT_MS: u64 = 30000; // TODO possibly tweak the WINDOW_SIZE in signal_tile.rs for better performance, temp fix! -Erik
