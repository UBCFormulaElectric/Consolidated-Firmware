pub mod clients;
pub mod subtable;


use std::sync::Arc;
use tokio::sync::RwLock;
use jsoncan_rust::can_database::CanDatabase;
use crate::tasks::client_api::clients::Clients;

#[derive(Clone)]
pub struct AppState {
    pub can_db: Arc<CanDatabase>,
    pub clients: Arc<RwLock<Clients>>,
}
