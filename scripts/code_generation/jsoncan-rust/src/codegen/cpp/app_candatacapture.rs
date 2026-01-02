use crate::{can_database::CanDatabase, codegen::cpp::CPPGenerator};

pub struct AppCanDataCaptureModule<'a> {
    pub can_db: &'a CanDatabase,
}

impl CPPGenerator for AppCanDataCaptureModule<'_> {
    fn header_template(&self) -> String {
        todo!()
    }
    fn source_template(&self) -> String {
        todo!()
    }
}
