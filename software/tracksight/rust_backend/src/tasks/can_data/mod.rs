
pub mod live_data_handler;
pub mod influx_handler;
pub mod can_data_handler;

use jsoncan_rust::{parsing::JsonCanParser, can_database::CanDatabase};
use crate::config::CONFIG;
pub fn load_can_database() -> Option<CanDatabase> {
    let parser = JsonCanParser::new(CONFIG.jsoncan_config_path.clone());
    match CanDatabase::from(parser) {
        Ok(db) => return Some(db),
        Err(e) => {
            eprintln!("Failed to load CAN database: {:?}", e);
            return None;
        }
    };
}