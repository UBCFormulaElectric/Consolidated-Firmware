use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
    reroute::CanTxConfig,
};

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canTx.c.j2")]
struct AppCanTxModuleSource {
    tx_messages: Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canTx.h.j2")]
struct AppCanTxModuleHeader {
    tx_messages: Vec<CanMessage>,
}

pub struct AppCanTxModule<'a> {
    pub can_db: &'a CanDatabase,
    pub tx_config: &'a CanTxConfig,
}

impl CPPGenerator for AppCanTxModule<'_> {
    fn header_template(&self) -> String {
        // let tx_messages: Vec<CanMessage> = self
        //     .can_db.
        // .messages
        // .values()
        // .filter(|msg| self.tx_config.tx_message_ids.contains(&msg.id))
        // .cloned()
        // .collect::<Vec<CanMessage>>();

        // let tx_messages = self.tx_config.

        AppCanTxModuleHeader { tx_messages }.render().unwrap()
    }
    fn source_template(&self) -> String {
        let tx_messages: Vec<CanMessage> = self
            .can_db
            .messages
            .values()
            .filter(|msg| self.tx_config.tx_message_ids.contains(&msg.id))
            .cloned()
            .collect::<Vec<CanMessage>>();

        AppCanTxModuleSource { tx_messages }.render().unwrap()
    }
}
