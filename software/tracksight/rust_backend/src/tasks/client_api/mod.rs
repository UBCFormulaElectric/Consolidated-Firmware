pub mod clients;
pub mod subtable_api_handler;
pub mod signal_api_handler;


use std::sync::Arc;
use tokio::sync::RwLock;
use jsoncan_rust::can_database::CanDatabase;
use crate::tasks::client_api::clients::Clients;

#[derive(Clone)]
pub struct AppState {
    pub can_db: Arc<CanDatabase>,
    pub clients: Arc<RwLock<Clients>>,
}
