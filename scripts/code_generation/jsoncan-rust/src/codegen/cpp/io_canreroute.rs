use askama::Template;

use crate::{
    can_database::{CanBus, CanDatabase},
    codegen::cpp::{CPPGenerator, id_macro},
    reroute::CanForward,
};

#[derive(Template)]
#[template(path = "io_canReroute.cpp.j2")]
struct IoCanRerouteModuleSource<'a> {
    can_db: &'a CanDatabase,
    node_busses: &'a Vec<&'a CanBus>,
    reroutes: &'a CanForward,
}
impl IoCanRerouteModuleSource<'_> {
    fn id_macro(self: &Self, msg_id: &u32) -> String {
        let name = self.can_db.get_message_name_by_id(*msg_id).unwrap();
        id_macro(&name)
    }
}

#[derive(Template)]
#[template(path = "io_canReroute.hpp.j2")]
struct IoCanRerouteModuleHeader<'a> {
    node_busses: &'a Vec<&'a CanBus>,
}

pub struct IoCanRerouteModule<'a> {
    can_db: &'a CanDatabase,
    reroute_config: &'a CanForward,
    node_busses: Vec<&'a CanBus>,
}
impl IoCanRerouteModule<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        board: &String,
        reroute_config: &'a CanForward,
    ) -> IoCanRerouteModule<'a> {
        let node_busses: Vec<&CanBus> = can_db
            .buses
            .iter()
            .filter(|b| b.node_names.contains(board))
            .collect();

        IoCanRerouteModule {
            can_db,
            reroute_config,
            node_busses: node_busses,
        }
    }
}

impl CPPGenerator for IoCanRerouteModule<'_> {
    fn file_stem(&self) -> String {
        "io_canReroute".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        IoCanRerouteModuleHeader {
            node_busses: &self.node_busses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        IoCanRerouteModuleSource {
            can_db: &self.can_db,
            node_busses: &self.node_busses,
            reroutes: self.reroute_config,
        }
        .render()
    }
}
