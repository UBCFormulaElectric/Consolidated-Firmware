use crate::{can_database::CanDatabase, reroute::CanRxConfig};
use std::collections::HashMap;

pub struct DbcGenerator<'a> {
    database: &'a CanDatabase,
    rx_configs: &'a HashMap<String, CanRxConfig>,
}

impl DbcGenerator<'_> {
    pub fn new<'a>(
        db: &'a CanDatabase,
        rx_configs: &'a HashMap<String, CanRxConfig>,
    ) -> DbcGenerator<'a> {
        DbcGenerator {
            database: db,
            rx_configs: rx_configs,
        }
    }

    pub fn source(&self) -> String {
        // Implementation to generate DBC source code
        todo!()
    }
}
