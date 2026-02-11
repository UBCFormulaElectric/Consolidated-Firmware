use std::collections::HashMap;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
    reroute::CanRxConfig,
};
use askama::Template;

#[derive(Template)]
#[template(path = "app_canRx.cpp.j2")]
struct AppCanRxModuleSource<'a> {
    node: &'a String,
    boards_messages: &'a HashMap<String, Vec<CanMessage>>, // maps messages to the board they came from
}

#[derive(Template)]
#[template(path = "app_canRx.hpp.j2")]
struct AppCanRxModuleHeader<'a> {
    boards_messages: &'a HashMap<String, Vec<CanMessage>>, // maps messages to the board they came from
}

pub struct AppCanRxModule<'a> {
    node: &'a String,
    boards_messages: HashMap<String, Vec<CanMessage>>, // maps messages to the board they came from
}

impl AppCanRxModule<'_> {
    pub fn new<'a>(
        can_db: &CanDatabase,
        node: &'a String,
        rx_config: &CanRxConfig,
    ) -> AppCanRxModule<'a> {
        let mut boards_messages: HashMap<String, Vec<CanMessage>> = HashMap::new();
        for msg_id in rx_config.get_all_msgs() {
            let msg = can_db.get_message_by_id(msg_id).unwrap();
            boards_messages
                .entry(msg.tx_node_name.clone())
                .or_insert(Vec::new())
                .push(msg);
        }

        AppCanRxModule {
            boards_messages,
            node,
        }
    }
}

impl CPPGenerator for AppCanRxModule<'_> {
    fn file_stem(&self) -> String {
        "app_canRx".to_string()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanRxModuleSource {
            node: self.node,
            boards_messages: &self.boards_messages,
        }
        .render()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanRxModuleHeader {
            boards_messages: &self.boards_messages,
        }
        .render()
    }
}
