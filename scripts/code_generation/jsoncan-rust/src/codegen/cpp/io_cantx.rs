use askama::Template;

use crate::{
    can_database::{CanBus, CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
    reroute::CanTxConfig,
};

#[derive(Template)]
#[template(path = "io_canTx.cpp.j2")]
struct IoCanTxModuleSource<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    board_tx_msgs_and_busses: &'a Vec<(CanMessage, Vec<String>)>,
}

#[derive(Template)]
#[template(path = "io_canTx.hpp.j2")]
struct IoCanTxModuleHeader<'a> {
    node_buses: &'a Vec<&'a CanBus>,
    board_tx_msgs_and_busses: &'a Vec<(CanMessage, Vec<String>)>,
}

pub struct IoCanTxModule<'a> {
    node_buses: Vec<&'a CanBus>,
    board_tx_msgs_and_busses: Vec<(CanMessage, Vec<String>)>,
}
impl IoCanTxModule<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        board: &String,
        tx_config: &'a CanTxConfig,
    ) -> IoCanTxModule<'a> {
        IoCanTxModule {
            board_tx_msgs_and_busses: tx_config
                .get_all()
                .into_iter()
                .map(|(m_id, busses)| (can_db.get_message_by_id(m_id).unwrap(), busses))
                .collect(),
            node_buses: can_db
                .buses
                .iter()
                .filter(|b| b.node_names.contains(board))
                .collect(),
        }
    }
}

impl CPPGenerator for IoCanTxModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleHeader {
            node_buses: &self.node_buses,
            board_tx_msgs_and_busses: &self.board_tx_msgs_and_busses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        IoCanTxModuleSource {
            node_buses: &self.node_buses,
            board_tx_msgs_and_busses: &self.board_tx_msgs_and_busses,
        }
        .render()
    }
    fn file_stem(&self) -> String {
        "io_canTx".to_string()
    }
}
