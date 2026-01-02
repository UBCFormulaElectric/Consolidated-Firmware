use crate::{
    can_database::CanDatabase,
    codegen::cpp::CPPGenerator,
    reroute::{CanRxConfig, CanTxConfig},
};

pub struct AppCanUtilsModule<'a> {
    pub can_db: &'a CanDatabase,
    pub tx_config: &'a CanTxConfig,
    pub rx_config: &'a CanRxConfig,
}

impl CPPGenerator for AppCanUtilsModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
