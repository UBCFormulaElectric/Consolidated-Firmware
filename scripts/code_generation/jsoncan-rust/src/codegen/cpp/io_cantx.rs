use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator, reroute::CanTxConfig};

pub struct IoCanTxModule<'a> {
    pub can_db: &'a CanDatabase,
    pub board: &'a String,
    pub tx_config: &'a CanTxConfig,
}

impl CPPGenerator for IoCanTxModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
