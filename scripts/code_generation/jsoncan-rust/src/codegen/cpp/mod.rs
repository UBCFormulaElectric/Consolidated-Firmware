use std::collections::HashSet;

use crate::{
    can_database::CanDatabase,
    reroute::{CanForward, CanRxConfig, CanTxConfig},
};

pub enum CModule<'a> {
    AppCanUtilsModule {
        can_db: &'a CanDatabase,
        tx_config: &'a CanTxConfig,
        rx_config: &'a CanRxConfig,
    },
    AppCanTxModule {
        can_db: &'a CanDatabase,
        tx_config: &'a CanTxConfig,
    },
    AppCanAlertsModule {
        can_db: &'a CanDatabase,
        board: &'a String,
    },
    AppCanDataCaptureModule {
        can_db: &'a CanDatabase,
    },
    AppCanRxModule {
        can_db: &'a CanDatabase,
        board: &'a String,
        rx_config: &'a CanRxConfig,
    },
    IoCanTxModule {
        can_db: &'a CanDatabase,
        board: &'a String,
        tx_config: &'a CanTxConfig,
    },
    IoCanRxModule {
        can_db: &'a CanDatabase,
        board: &'a String,
        rx_config: &'a CanRxConfig,
    },
    IoCanRerouteModule {
        can_db: &'a CanDatabase,
        board: &'a String,
        reroute_config: &'a HashSet<CanForward>,
    },
}

impl CModule<'_> {
    pub fn header_template(&self) -> String {
        todo!()
    }
    pub fn source_template(&self) -> String {
        todo!()
    }
}
