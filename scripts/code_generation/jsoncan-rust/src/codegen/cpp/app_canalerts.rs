use askama::Template;

use crate::{
    can_database::{CanAlert, CanDatabase, GroupedAlerts},
    codegen::cpp::CPPGenerator,
};

impl GroupedAlerts {
    pub fn flatten(self: &Self) -> Vec<CanAlert> {
        let mut all_alerts: Vec<CanAlert> = Vec::new();
        all_alerts.extend(self.infos.clone());
        all_alerts.extend(self.warnings.clone());
        all_alerts.extend(self.faults.clone());
        all_alerts
    }
}

#[derive(Template)]
#[template(path = "app_canAlerts.cpp.j2")]
struct AppCanAlertsModuleSource<'a> {
    all_alerts: Vec<CanAlert>,
    node_name_and_alerts: &'a Vec<(String, &'a GroupedAlerts)>,
    node_name: &'a String,
}

#[derive(Template)]
#[template(path = "app_canAlerts.hpp.j2")]
struct AppCanAlertsModuleHeader<'a> {
    node_tx_alerts: &'a GroupedAlerts,
    node_name: &'a String,
}

pub struct AppCanAlertsModule<'a> {
    node_name: &'a String,
    node_name_and_alerts: Vec<(String, &'a GroupedAlerts)>,
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
                node_name_and_alerts: Vec::new(),
            };
        }
        AppCanAlertsModule {
            node_name,
            node_name_and_alerts: can_db
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
        if self.node_name_and_alerts.is_empty() {
            return Ok("".to_string());
        }
        AppCanAlertsModuleHeader {
            node_tx_alerts: &self
                .node_name_and_alerts
                .iter()
                .find(|(a, _b)| a == self.node_name)
                .unwrap()
                .1,
            node_name: self.node_name,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        if self.node_name_and_alerts.is_empty() {
            return Ok("".to_string());
        }
        let node_tx_alerts = self
            .node_name_and_alerts
            .iter()
            .find(|(a, _b)| a == self.node_name)
            .unwrap()
            .1;
        AppCanAlertsModuleSource {
            all_alerts: node_tx_alerts.flatten(),
            node_name_and_alerts: &self.node_name_and_alerts,
            node_name: self.node_name,
        }
        .render()
    }
    fn file_stem(&self) -> String {
        "app_canAlerts".to_string()
    }
}
