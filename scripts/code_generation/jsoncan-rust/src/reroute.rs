use crate::can_database::CanDatabase;
use std::collections::{HashMap, HashSet};

pub struct CanRxConfig {}

pub struct CanTxConfig {}

pub struct CanForward {}

pub fn resolve_tx_rx_reroute(
    can_db: &CanDatabase,
) -> (
    HashMap<String, CanTxConfig>,         // tx config per node
    HashMap<String, CanRxConfig>,         // rx config per node
    HashMap<String, HashSet<CanForward>>, // forwards per node
) {
    todo!()
}
