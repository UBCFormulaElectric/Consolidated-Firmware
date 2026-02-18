use askama::Template;

use crate::{
    can_database::{CanDatabase, NodeAlerts},
    codegen::cpp::CPPGenerator,
};

#[derive(Template)]
#[template(path = "app_canAlerts.cpp.j2")]
struct AppCanAlertsModuleSource<'a>{
    board_alerts: &'a NodeAlerts,
    all_node_name_and_alerts: &'a Vec<(String, &'a NodeAlerts)>,
    node_name: &'a String,
}

#[derive(Template)]
#[template(path = "app_canAlerts.hpp.j2")]
struct AppCanAlertsModuleHeader<'a> {
    board_alerts: &'a NodeAlerts,
    node_name: &'a String,
}

pub struct AppCanAlertsModule<'a> {
    all_node_name_and_alerts: Vec<(String, &'a NodeAlerts)>,
    node_name: &'a String,
}

impl AppCanAlertsModule<'_> {
    pub fn new<'a>(can_db: &'a CanDatabase, node_name: &'a String) -> AppCanAlertsModule<'a> {
        let self_alerts= &can_db
                .nodes
                .iter()
                .find(|n| n.name == *node_name)
                .unwrap()
                .alerts;
        if self_alerts.is_none() {
             return AppCanAlertsModule {
                node_name,
                all_node_name_and_alerts: Vec::new(),
            };
        }
        AppCanAlertsModule {
            node_name,
            all_node_name_and_alerts: can_db
                .nodes
                .iter()
                .filter_map(|n| match n.alerts {
                    Some(ref alerts) => Some((n.name.clone(), alerts)),
                    None => None,
                })
                .collect(),
        }
    }
}

impl CPPGenerator for AppCanAlertsModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        if self.all_node_name_and_alerts.is_empty() {
            return Ok("".to_string());
        }
        AppCanAlertsModuleHeader {
            board_alerts: &self
                .all_node_name_and_alerts
                .iter()
                .find(|(a, _b)| a == self.node_name)
                .unwrap()
                .1,
            node_name: self.node_name,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        if self.all_node_name_and_alerts.is_empty() {
            return Ok("".to_string());
        }
        AppCanAlertsModuleSource {
            board_alerts: self
                .all_node_name_and_alerts
                .iter()
                .find(|(a, _b)| a == self.node_name)
                .unwrap()
                .1,
            all_node_name_and_alerts: &self.all_node_name_and_alerts,
            node_name: self.node_name,
        }
        .render()
    }
    fn file_stem(&self) -> String {
        "app_canAlerts".to_string()
    }
}
