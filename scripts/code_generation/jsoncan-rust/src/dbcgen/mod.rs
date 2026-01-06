use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    reroute::CanRxConfig,
};
use std::collections::HashMap;

const DBC_DEFAULT_RECEIVER: &str = "DEBUG";

#[derive(Template)]
#[template(path = "../src/dbcgen/dbc.txt.j2")] // NOTE it is very important that the extension is txt otherwise the templater shits itself
struct DBCTemplate<'a> {
    can_db: &'a CanDatabase,
    nodes_list: &'a Vec<String>,
    messages: &'a Vec<CanMessage>,
    rx_configs: &'a HashMap<String, CanRxConfig>,
}
impl DBCTemplate<'_> {
    fn receivers(self: &Self, msg: &CanMessage) -> String {
        let mut out = self.rx_configs[&msg.tx_node_name].get_busses_for_msg(msg.id);
        if !out.contains(&DBC_DEFAULT_RECEIVER.to_string()) {
            out.push(DBC_DEFAULT_RECEIVER.to_string());
        }
        out.join(" ")
    }

    fn enumfuckshit(self: &Self, enum_name: &str) -> String {
        self.can_db
            .get_enum(enum_name)
            .unwrap()
            .values
            .iter()
            .map(|(item_name, item_value)| format!("{} \"{}\"", item_value, item_name))
            .collect::<Vec<String>>()
            .join(" ")
    }
}

pub struct DbcGenerator<'a> {
    node_name_list: Vec<String>,
    messages: Vec<CanMessage>,
    rx_configs: &'a HashMap<String, CanRxConfig>,
    can_db: &'a CanDatabase,
}

impl DbcGenerator<'_> {
    pub fn new<'a>(
        can_db: &'a CanDatabase,
        rx_configs: &'a HashMap<String, CanRxConfig>,
    ) -> DbcGenerator<'a> {
        DbcGenerator {
            node_name_list: can_db
                .nodes
                .iter()
                .map(|n| n.name.clone())
                .chain(std::iter::once(DBC_DEFAULT_RECEIVER.to_string()))
                .collect::<Vec<String>>(),
            messages: can_db.get_all_msgs().expect("surely"),
            rx_configs,
            can_db,
        }
    }

    pub fn source(&self) -> Result<String, askama::Error> {
        DBCTemplate {
            can_db: &self.can_db,
            nodes_list: &self.node_name_list,
            messages: &self.messages,
            rx_configs: &self.rx_configs,
        }
        .render()
    }
}
