use askama::Template;

use crate::can_database::CanMessage;
use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator, reroute::CanRxConfig};

#[derive(Template)]
#[template(path = "io_canRx.c.j2")]
struct IoCanRxModuleSource<'a> {
    messages_on_busses: &'a Vec<(String, Vec<CanMessage>)>,
}

#[derive(Template)]
#[template(path = "io_canRx.h.j2")]
struct IoCanRxModuleHeader<'a> {
    node: &'a String,
    messages_on_busses: &'a Vec<(String, Vec<CanMessage>)>,
}

pub struct IoCanRxModule<'a> {
    board: &'a String,
    messages_on_busses: Vec<(String, Vec<CanMessage>)>,
}
impl IoCanRxModule<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        rx_config: &'a CanRxConfig,
        board: &'a String,
    ) -> IoCanRxModule<'a> {
        let mut messages_on_busses: Vec<(String, Vec<CanMessage>)> = Vec::new();
        for bus_name in rx_config.get_all_busses() {
            let msgs = rx_config.get_msgs_on_bus(&bus_name);
            messages_on_busses.push((
                bus_name.clone(),
                msgs.iter()
                    .map(|m| can_db.get_message_by_id(*m).unwrap())
                    .collect(),
            ));
        }
        IoCanRxModule {
            board,
            messages_on_busses,
        }
    }
}

impl CPPGenerator for IoCanRxModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        IoCanRxModuleHeader {
            node: self.board,
            messages_on_busses: &self.messages_on_busses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        IoCanRxModuleSource {
            messages_on_busses: &self.messages_on_busses,
        }
        .render()
    }
}
