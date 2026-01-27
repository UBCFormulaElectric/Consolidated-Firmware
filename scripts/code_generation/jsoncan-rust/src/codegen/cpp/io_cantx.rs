use askama::Template;

use crate::{
    can_database::{CanBus, CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
    reroute::CanTxConfig,
};

#[derive(Template)]
#[template(path = "io_canTx.c.j2")]
struct IoCanTxModuleSource<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    messages: &'a Vec<(CanMessage, Vec<String>)>,
}

#[derive(Template)]
#[template(path = "io_canTx.h.j2")]
struct IoCanTxModuleHeader<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    messages: &'a Vec<(CanMessage, Vec<String>)>,
    fd: bool,
}

pub struct IoCanTxModule<'a> {
    messages: Vec<(CanMessage, Vec<String>)>,
    node_buses: Vec<&'a CanBus>,
    fd: bool,
}
impl IoCanTxModule<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        board: &'a String,
        tx_config: &'a CanTxConfig,
    ) -> IoCanTxModule<'a> {
        let node_buses: Vec<&'a CanBus> = can_db
            .buses
            .iter()
            .filter(|b| b.node_names.contains(board))
            .collect();
        IoCanTxModule {
            messages: tx_config
                .get_all()
                .iter()
                .map(|(m_id, busses)| (can_db.get_message_by_id(*m_id).unwrap(), busses.clone()))
                .collect(),
            fd: node_buses.iter().any(|b| b.fd),
            node_buses,
        }
    }
}

impl CPPGenerator for IoCanTxModule<'_> {
    fn file_stem(&self) -> String {
        "io_canTx".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleHeader {
            messages: &self.messages,
            fd: self.fd,
            node_buses: &self.node_buses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleSource {
            node_buses: &self.node_buses,
            messages: &self.messages,
        }
        .render()
    }
}

#[derive(Template)]
#[template(path = "io_canTx.cpp.j2")]
struct IoCanTxModuleSourceCpp<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    messages: &'a Vec<(CanMessage, Vec<String>)>,
}

#[derive(Template)]
#[template(path = "io_canTx.hpp.j2")]
struct IoCanTxModuleHeaderCpp<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    messages: &'a Vec<(CanMessage, Vec<String>)>,
    fd: bool,
}

pub struct IoCanTxModuleCpp<'a> {
    messages: Vec<(CanMessage, Vec<String>)>,
    node_buses: Vec<&'a CanBus>,
    fd: bool,
}
impl IoCanTxModuleCpp<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        board: &'a String,
        tx_config: &'a CanTxConfig,
    ) -> IoCanTxModuleCpp<'a> {
        let node_buses: Vec<&'a CanBus> = can_db
            .buses
            .iter()
            .filter(|b| b.node_names.contains(board))
            .collect();
        IoCanTxModuleCpp {
            messages: tx_config
                .get_all()
                .iter()
                .map(|(m_id, busses)| (can_db.get_message_by_id(*m_id).unwrap(), busses.clone()))
                .collect(),
            fd: node_buses.iter().any(|b| b.fd),
            node_buses,
        }
    }
}

impl CPPGenerator for IoCanTxModuleCpp<'_> {
    fn file_stem(&self) -> String {
        "io_canTx".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleHeaderCpp {
            messages: &self.messages,
            fd: self.fd,
            node_buses: &self.node_buses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleSourceCpp {
            node_buses: &self.node_buses,
            messages: &self.messages,
        }
        .render()
    }
}