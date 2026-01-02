use std::collections::HashSet;

use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator, reroute::CanForward};

pub struct IoCanRerouteModule<'a> {
    pub can_db: &'a CanDatabase,
    pub board: &'a String,
    pub reroute_config: &'a HashSet<CanForward>,
}

impl CPPGenerator for IoCanRerouteModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
