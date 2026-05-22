use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "app_canDataCapture.cpp.j2")]
struct AppCanDataCaptureModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
    enabled: bool,
}

#[derive(Template)]
#[template(path = "app_canDataCapture.hpp.j2")]
struct AppCanDataCaptureModuleHeader {}

pub struct AppCanDataCaptureModule {
    messages: Vec<CanMessage>,
    enabled: bool,
}

impl AppCanDataCaptureModule {
    pub fn new(can_db: &CanDatabase, node_name: &str) -> Self {
        Self {
            messages: can_db.get_all_msgs().expect("Failed to get all messages"),
            enabled: can_db
                .data_capture_nodes
                .iter()
                .any(|n| n == node_name),
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
            enabled: self.enabled,
        }
        .render()
    }
    fn file_stem(&self) -> String {
        "app_canDataCapture".to_string()
    }
}
