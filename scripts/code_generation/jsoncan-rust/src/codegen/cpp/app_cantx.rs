use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "app_canTx.c.j2")]
struct AppCanTxModuleSource<'a> {
    tx_messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "app_canTx.h.j2")]
struct AppCanTxModuleHeader<'a> {
    tx_messages: &'a Vec<CanMessage>,
}

pub struct AppCanTxModule {
    tx_messages: Vec<CanMessage>,
}

impl AppCanTxModule {
    pub fn new(can_db: &CanDatabase, node_name: &String) -> Self {
        Self {
            tx_messages: can_db.get_message_by_node(node_name).unwrap(),
        }
    }
}

impl CPPGenerator for AppCanTxModule {
    fn file_stem(&self) -> String {
        "app_canTx".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanTxModuleHeader {
            tx_messages: &self.tx_messages,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanTxModuleSource {
            tx_messages: &self.tx_messages,
        }
        .render()
    }
}

#[derive(Template)]
#[template(path = "app_canTx.cpp.j2")]
struct AppCanTxModuleSourceCpp<'a> {
    tx_messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "app_canTx.hpp.j2")]
struct AppCanTxModuleHeaderCpp<'a> {
    tx_messages: &'a Vec<CanMessage>,
}

pub struct AppCanTxModuleCpp {
    tx_messages: Vec<CanMessage>,
}

impl AppCanTxModuleCpp {
    pub fn new(can_db: &CanDatabase, node_name: &String) -> Self {
        Self {
            tx_messages: can_db.get_message_by_node(node_name).unwrap(),
        }
    }
}

impl CPPGenerator for AppCanTxModuleCpp {
    fn file_stem(&self) -> String {
        "app_canTx".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanTxModuleHeaderCpp {
            tx_messages: &self.tx_messages,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanTxModuleSourceCpp {
            tx_messages: &self.tx_messages,
        }
        .render()
    }
}