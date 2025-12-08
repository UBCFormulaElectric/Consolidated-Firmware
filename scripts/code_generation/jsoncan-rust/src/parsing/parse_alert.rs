use crate::can_database::{CanAlert, CanMessage};

pub fn parse_alert_data(
    can_data_dir: &str,
    node_name: &str,
) -> Option<(Vec<CanMessage>, Vec<CanAlert>)> {
    todo!()
}
