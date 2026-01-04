use askama::Template;

use crate::{
    can_database::{CanAlert, CanAlertType, CanDatabase},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canAlerts.c.j2")]
struct AppCanAlertsModuleSource {
    node_tx_alerts: Vec<CanAlert>,
    node_name_and_alerts: Vec<(String, Vec<CanAlert>)>,
    node_name: String,
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canAlerts.h.j2")]
struct AppCanAlertsModuleHeader {
    tx_info: Vec<CanAlert>,
    tx_warnings: Vec<CanAlert>,
    tx_faults: Vec<CanAlert>,
    node_name: String,
}

pub struct AppCanAlertsModule<'a> {
    pub can_db: &'a CanDatabase,
    pub board: &'a String,
}

impl CPPGenerator for AppCanAlertsModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        todo!()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        todo!()
    }
}
