pub mod app_canalerts;
pub mod app_candatacapture;
pub mod app_canrx;
pub mod app_cantx;
pub mod app_canutil;
pub mod io_canreroute;
pub mod io_canrx;
pub mod io_cantx;

pub use app_canalerts::AppCanAlertsModule;
pub use app_candatacapture::AppCanDataCaptureModule;
pub use app_canrx::AppCanRxModule;
pub use app_cantx::AppCanTxModule;
pub use app_canutil::AppCanUtilsModule;
pub use io_canreroute::IoCanRerouteModule;
pub use io_canrx::IoCanRxModule;
pub use io_cantx::IoCanTxModule;

use crate::can_database::{CanMessage, CanSignal};

use convert_case::{Case, Casing};

trait CPPGenerator {
    fn header_template(&self) -> Result<String, askama::Error>;
    fn source_template(&self) -> Result<String, askama::Error>;
}

pub enum CPPModule<'a> {
    AppCanUtilsModule(AppCanUtilsModule),
    AppCanTxModule(AppCanTxModule),
    AppCanAlertsModule(AppCanAlertsModule<'a>),
    AppCanDataCaptureModule(AppCanDataCaptureModule),
    AppCanRxModule(AppCanRxModule<'a>),
    IoCanTxModule(IoCanTxModule<'a>),
    IoCanRxModule(IoCanRxModule<'a>),
    IoCanRerouteModule(IoCanRerouteModule<'a>),
}

impl CPPModule<'_> {
    pub fn header_template(&self) -> Result<String, askama::Error> {
        match self {
            CPPModule::AppCanUtilsModule(module) => module.header_template(),
            CPPModule::AppCanTxModule(module) => module.header_template(),
            CPPModule::AppCanAlertsModule(module) => module.header_template(),
            CPPModule::AppCanDataCaptureModule(module) => module.header_template(),
            CPPModule::AppCanRxModule(module) => module.header_template(),
            CPPModule::IoCanTxModule(module) => module.header_template(),
            CPPModule::IoCanRxModule(module) => module.header_template(),
            CPPModule::IoCanRerouteModule(module) => module.header_template(),
        }
    }
    pub fn source_template(&self) -> Result<String, askama::Error> {
        match self {
            CPPModule::AppCanUtilsModule(module) => module.source_template(),
            CPPModule::AppCanTxModule(module) => module.source_template(),
            CPPModule::AppCanAlertsModule(module) => module.source_template(),
            CPPModule::AppCanDataCaptureModule(module) => module.source_template(),
            CPPModule::AppCanRxModule(module) => module.source_template(),
            CPPModule::IoCanTxModule(module) => module.source_template(),
            CPPModule::IoCanRxModule(module) => module.source_template(),
            CPPModule::IoCanRerouteModule(module) => module.source_template(),
        }
    }
}

impl CanSignal {
    fn snake_name(self: &Self) -> String {
        self.name.to_case(Case::Snake)
    }

    fn screaming_snake_name(self: &Self) -> String {
        self.name.to_case(Case::Snake).to_uppercase()
    }

    pub fn start_val_macro(self: &Self) -> String {
        format!("CANSIG_{}_START_VAL", self.snake_name().to_uppercase())
    }

    pub fn max_val_macro(self: &Self) -> String {
        format!("CANSIG_{}_MAX_VAL", self.snake_name().to_uppercase())
    }

    pub fn min_val_macro(self: &Self) -> String {
        format!("CANSIG_{}_MIN_VAL", self.snake_name().to_uppercase())
    }

    pub fn scale_macro(self: &Self) -> String {
        format!("CANSIG_{}_SCALE", self.snake_name().to_uppercase())
    }

    pub fn offset_macro(self: &Self) -> String {
        format!("CANSIG_{}_OFFSET", self.snake_name().to_uppercase())
    }

    pub fn datatype(self: &Self) -> String {
        todo!()
    }

    pub fn representation(self: &Self) -> String {
        todo!()
    }
}

pub fn id_macro(name: &str) -> String {
    format!("CAN_MSG_{}_ID", name.to_case(Case::Snake).to_uppercase())
}

impl CanMessage {
    fn c_type(self: &Self) -> String {
        format!("{}_Signals", self.name)
    }

    fn snake_name(&self) -> String {
        self.name.to_case(Case::Snake)
    }

    fn screaming_snake_name(&self) -> String {
        self.name.to_case(Case::Snake).to_uppercase()
    }

    pub fn id_macro(&self) -> String {
        id_macro(&self.name)
    }

    pub fn dlc_macro(&self) -> String {
        format!("CAN_MSG_{}_DLC", self.snake_name().to_uppercase())
    }

    pub fn cycle_time_macro(&self) -> String {
        format!("CAN_MSG_{}_CYCLE_TIME_MS", self.snake_name().to_uppercase())
    }
}
