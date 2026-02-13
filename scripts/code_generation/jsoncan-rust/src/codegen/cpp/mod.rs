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

use crate::can_database::{CanMessage, CanSignal, CanSignalType};

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
        match self.signal_type {
            CanSignalType::Numerical => {
                if self.scale != 1.0 || self.offset != 0.0 {
                    "float".to_string()
                }
                else if self.signed {
                    match self.bits {
                        0_u16..=8_u16 => "int8_t".to_string(),
                        9_u16..=16_u16 => "int16_t".to_string(),
                        17_u16..=32_u16 => "int32_t".to_string(),
                        33_u16..=64_u16 => "int64_t".to_string(),
                        _ => "int".to_string(), // Fallback for unusual bit lengths
                    }
                } else {
                    match self.bits {
                        0_u16..=8_u16 => "uint8_t".to_string(),
                        9_u16..=16_u16 => "uint16_t".to_string(),
                        17_u16..=32_u16 => "uint32_t".to_string(),
                        33_u16..=64_u16 => "uint64_t".to_string(),
                        _ => "unsigned int".to_string(), // Fallback for unusual bit lengths
                    }
                }
            },
            CanSignalType::Boolean => "bool".to_string(),
            CanSignalType::Enum => self.enum_name.as_ref().expect("Enum must have name").clone(),
            CanSignalType::Alert => "alert".to_string(),
        }
    }

    pub fn representation(self: &Self) -> String {
        if self.scale != 1.0 || self.offset != 0.0 {
            return "float".to_string();
        }
        if self.signed {
            return "int32_t".to_string();
        }
        "uint32_t".to_string()
    }

    pub fn endian_macro(self: &Self) -> String {
        if self.big_endian {
            "@0".to_string()
        } else {
            "@1".to_string()
        }
    }

    pub fn signed_macro(self: &Self) -> String {
        if self.signed {
            "-".to_string()
        } else {
            "+".to_string()
        }
    }

    pub fn start_val_name(self: &Self) -> String {
        // format!("{}_START_VAL", self.snake_name().to_uppercase())
        match self.signal_type {
            CanSignalType::Numerical => {
                self.start_val.to_string()
            },
            CanSignalType::Boolean => {
                if self.start_val == 0f64 {
                    "false".to_string()
                } else {
                    "true".to_string()
                }
            },
            CanSignalType::Enum => todo!(),
            CanSignalType::Alert => "false".to_string(),
        }
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
