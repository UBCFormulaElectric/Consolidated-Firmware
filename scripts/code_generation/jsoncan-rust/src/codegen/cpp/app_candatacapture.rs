use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "app_canDataCapture.cpp.j2")]
struct AppCanDataCaptureModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "app_canDataCapture.hpp.j2")]
struct AppCanDataCaptureModuleHeader {}

pub struct AppCanDataCaptureModule {
    messages: Vec<CanMessage>,
}

impl AppCanDataCaptureModule {
    pub fn new(can_db: &CanDatabase, node_name: &str) -> Self {
        // DAM needs all messages, other boards technically don't need this file but legacy had it included
        let messages = if node_name == "DAM" {
            can_db.get_all_msgs().expect("Failed to get all messages")
        } else {
            can_db
                .get_message_by_node(node_name)
                .expect("Node not found")
        };
        Self { messages }
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
    fn file_stem(&self) -> String {
        "app_canDataCapture".to_string()
    }
}
