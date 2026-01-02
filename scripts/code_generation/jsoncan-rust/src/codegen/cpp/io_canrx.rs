use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator, reroute::CanRxConfig};

pub struct IoCanRxModule<'a> {
    pub can_db: &'a CanDatabase,
    pub board: &'a String,
    pub rx_config: &'a CanRxConfig,
}

impl CPPGenerator for IoCanRxModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
