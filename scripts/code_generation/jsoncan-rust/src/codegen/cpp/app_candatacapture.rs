use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canDataCapture.c.j2")]
struct AppCanDataCaptureModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canDataCapture.h.j2")]
struct AppCanDataCaptureModuleHeader {}

pub struct AppCanDataCaptureModule {
    messages: Vec<CanMessage>,
}

impl AppCanDataCaptureModule {
    pub fn new(can_db: &CanDatabase, node_name: &str) -> Self {
        Self {
            messages: can_db.get_message_by_node(node_name).expect("surely"),
        }
    }
}

impl CPPGenerator for AppCanDataCaptureModule {
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanDataCaptureModuleHeader {}.render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanDataCaptureModuleSource {
            messages: &self.messages,
        }
        .render()
    }
}
