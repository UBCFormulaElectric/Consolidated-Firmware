use askama::Template;

use crate::{
    can_database::{CanAlert, CanDatabase},
    codegen::cpp::CPPGenerator,
};

struct GroupedAlerts {
    info: Vec<CanAlert>,
    warnings: Vec<CanAlert>,
    faults: Vec<CanAlert>,
}

impl GroupedAlerts {
    pub fn flatten(self: &Self) -> Vec<CanAlert> {
        let mut all_alerts: Vec<CanAlert> = Vec::new();
        all_alerts.extend(self.info.clone());
        all_alerts.extend(self.warnings.clone());
        all_alerts.extend(self.faults.clone());
        all_alerts
    }
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canAlerts.c.j2")]
struct AppCanAlertsModuleSource<'a> {
    node_tx_alerts: Vec<CanAlert>,
    node_name_and_alerts: &'a Vec<(String, GroupedAlerts)>,
    node_name: &'a String,
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canAlerts.h.j2")]
struct AppCanAlertsModuleHeader<'a> {
    node_tx_alerts: &'a GroupedAlerts,
    node_name: &'a String,
}

pub struct AppCanAlertsModule<'a> {
    node_name: &'a String,
    node_name_and_alerts: Vec<(String, GroupedAlerts)>,
}

impl AppCanAlertsModule<'_> {
    pub fn new<'a>(can_db: &CanDatabase, node_name: &'a String) -> AppCanAlertsModule<'a> {
        assert!(
            can_db
                .nodes
                .iter()
                .find(|n| n.name == *node_name)
                .unwrap()
                .alerts
                .is_some(),
            "Tried to generate CAN Alerts module for node {} but it has no alerts defined",
            node_name
        );
        AppCanAlertsModule {
            node_name,
            node_name_and_alerts: can_db
                .nodes
                .iter()
                .filter_map(|n| match n.alerts {
                    Some(ref alerts) => Some((
                        n.name.clone(),
                        GroupedAlerts {
                            info: alerts.infos.alerts.clone(),
                            warnings: alerts.warnings.alerts.clone(),
                            faults: alerts.faults.alerts.clone(),
                        },
                    )),
                    None => None,
                })
                .collect(),
        }
    }
}

impl CPPGenerator for AppCanAlertsModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanAlertsModuleSource {
            node_tx_alerts: self
                .node_name_and_alerts
                .iter()
                .find(|(a, _b)| a == self.node_name)
                .unwrap()
                .1
                .flatten(),
            node_name_and_alerts: &self.node_name_and_alerts,
            node_name: self.node_name,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
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
}
