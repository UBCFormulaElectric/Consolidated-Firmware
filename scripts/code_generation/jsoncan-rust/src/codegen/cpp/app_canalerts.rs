use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator};

pub struct AppCanAlertsModule<'a> {
    pub can_db: &'a CanDatabase,
    pub board: &'a String,
}

impl CPPGenerator for AppCanAlertsModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
