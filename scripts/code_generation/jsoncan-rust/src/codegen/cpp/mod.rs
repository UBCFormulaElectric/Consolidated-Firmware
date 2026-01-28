pub mod app_canalerts;
pub mod app_candatacapture;
pub mod app_canrx;
pub mod app_cantx;
pub mod app_canutil;
pub mod io_canreroute;
pub mod io_canrx;
pub mod io_cantx;

pub use app_canalerts::{AppCanAlertsModule, AppCanAlertsModuleCpp};
pub use app_candatacapture::{AppCanDataCaptureModule, AppCanDataCaptureModuleCpp};
pub use app_canrx::{AppCanRxModule, AppCanRxModuleCpp};
pub use app_cantx::{AppCanTxModule, AppCanTxModuleCpp};
pub use app_canutil::{AppCanUtilsModule, AppCanUtilsModuleCpp};
pub use io_canreroute::{IoCanRerouteModule, IoCanRerouteModuleCpp};
pub use io_canrx::{IoCanRxModule, IoCanRxModuleCpp};
pub use io_cantx::{IoCanTxModule, IoCanTxModuleCpp};

use crate::can_database::{CanMessage, CanSignal};

use convert_case::{Case, Casing};

pub trait CPPGenerator {
    fn header_template(&self) -> Result<String, askama::Error>;
    fn source_template(&self) -> Result<String, askama::Error>;
    fn file_stem(&self) -> String;
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

impl CPPGenerator for CPPModule<'_> {
    fn file_stem(&self) -> String {
        match self {
            CPPModule::AppCanUtilsModule(module) => module.file_stem(),
            CPPModule::AppCanTxModule(module) => module.file_stem(),
            CPPModule::AppCanAlertsModule(module) => module.file_stem(),
            CPPModule::AppCanDataCaptureModule(module) => module.file_stem(),
            CPPModule::AppCanRxModule(module) => module.file_stem(),
            CPPModule::IoCanTxModule(module) => module.file_stem(),
            CPPModule::IoCanRxModule(module) => module.file_stem(),
            CPPModule::IoCanRerouteModule(module) => module.file_stem(),
        }
    }
    fn header_template(&self) -> Result<String, askama::Error> {
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
    fn source_template(&self) -> Result<String, askama::Error> {
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

pub enum CPPModuleCpp<'a> {
    AppCanUtilsModule(AppCanUtilsModuleCpp),
    AppCanTxModule(AppCanTxModuleCpp),
    AppCanAlertsModule(AppCanAlertsModuleCpp<'a>),
    AppCanDataCaptureModule(AppCanDataCaptureModuleCpp),
    AppCanRxModule(AppCanRxModuleCpp<'a>),
    IoCanTxModule(IoCanTxModuleCpp<'a>),
    IoCanRxModule(IoCanRxModuleCpp<'a>),
    IoCanRerouteModule(IoCanRerouteModuleCpp<'a>),
}

impl CPPGenerator for CPPModuleCpp<'_> {
    fn file_stem(&self) -> String {
        match self {
            CPPModuleCpp::AppCanUtilsModule(module) => module.file_stem(),
            CPPModuleCpp::AppCanTxModule(module) => module.file_stem(),
            CPPModuleCpp::AppCanAlertsModule(module) => module.file_stem(),
            CPPModuleCpp::AppCanDataCaptureModule(module) => module.file_stem(),
            CPPModuleCpp::AppCanRxModule(module) => module.file_stem(),
            CPPModuleCpp::IoCanTxModule(module) => module.file_stem(),
            CPPModuleCpp::IoCanRxModule(module) => module.file_stem(),
            CPPModuleCpp::IoCanRerouteModule(module) => module.file_stem(),
        }
    }
    fn header_template(&self) -> Result<String, askama::Error> {
        match self {
            CPPModuleCpp::AppCanUtilsModule(module) => module.header_template(),
            CPPModuleCpp::AppCanTxModule(module) => module.header_template(),
            CPPModuleCpp::AppCanAlertsModule(module) => module.header_template(),
            CPPModuleCpp::AppCanDataCaptureModule(module) => module.header_template(),
            CPPModuleCpp::AppCanRxModule(module) => module.header_template(),
            CPPModuleCpp::IoCanTxModule(module) => module.header_template(),
            CPPModuleCpp::IoCanRxModule(module) => module.header_template(),
            CPPModuleCpp::IoCanRerouteModule(module) => module.header_template(),
        }
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        match self {
            CPPModuleCpp::AppCanUtilsModule(module) => module.source_template(),
            CPPModuleCpp::AppCanTxModule(module) => module.source_template(),
            CPPModuleCpp::AppCanAlertsModule(module) => module.source_template(),
            CPPModuleCpp::AppCanDataCaptureModule(module) => module.source_template(),
            CPPModuleCpp::AppCanRxModule(module) => module.source_template(),
            CPPModuleCpp::IoCanTxModule(module) => module.source_template(),
            CPPModuleCpp::IoCanRxModule(module) => module.source_template(),
            CPPModuleCpp::IoCanRerouteModule(module) => module.source_template(),
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
        if self.scale != 1.0 || self.offset != 0.0 {
            return "float".to_string();
        }
        if self.signed {
            match self.bits {
                8 => "int8_t".to_string(),
                16 => "int16_t".to_string(),
                32 => "int32_t".to_string(),
                64 => "int64_t".to_string(),
                _ => "int".to_string(), // Fallback for unusual bit lengths
            }
        } else {
            match self.bits {
                8 => "uint8_t".to_string(),
                16 => "uint16_t".to_string(),
                32 => "uint32_t".to_string(),
                64 => "uint64_t".to_string(),
                _ => "unsigned int".to_string(), // Fallback for unusual bit lengths
            }
        }
    }

    pub fn representation(self: &Self) -> String {
        if self.scale != 1.0 || self.offset != 0.0 {
            return "float".to_string();
        }
        if self.signed {
            return "int".to_string();
        }
        "uint".to_string()
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