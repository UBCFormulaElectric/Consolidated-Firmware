use askama::Template;

use crate::{
    can_database::{CanDatabase, CanMessage},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "app_canDataCapture.c.j2")]
struct AppCanDataCaptureModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "app_canDataCapture.h.j2")]
struct AppCanDataCaptureModuleHeader {}

pub struct AppCanDataCaptureModule {
    messages: Vec<CanMessage>,
}

impl AppCanDataCaptureModule {
    pub fn new(can_db: &CanDatabase, node_name: &str) -> Self {
        Self {
            messages: can_db
                .get_message_by_node(node_name)
                .expect("Node not found"),
        }
    }
}

impl CPPGenerator for AppCanDataCaptureModule {
    fn file_stem(&self) -> String {
        "app_canDataCapture".to_string()
    }
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

#[derive(Template)]
#[template(path = "app_canDataCapture.cpp.j2")]
struct AppCanDataCaptureModuleSourceCpp<'a> {
    messages: &'a Vec<CanMessage>,
}

#[derive(Template)]
#[template(path = "app_canDataCapture.hpp.j2")]
struct AppCanDataCaptureModuleHeaderCpp {}

pub struct AppCanDataCaptureModuleCpp {
    messages: Vec<CanMessage>,
}

impl AppCanDataCaptureModuleCpp {
    pub fn new(can_db: &CanDatabase, node_name: &str) -> Self {
        Self {
            messages: can_db
                .get_message_by_node(node_name)
                .expect("Node not found"),
        }
    }
}

impl CPPGenerator for AppCanDataCaptureModuleCpp {
    fn file_stem(&self) -> String {
        "app_canDataCapture".to_string()
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanDataCaptureModuleHeaderCpp {}.render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanDataCaptureModuleSourceCpp {
            messages: &self.messages,
        }
        .render()
    }
}