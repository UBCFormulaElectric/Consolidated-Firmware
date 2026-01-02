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

trait CPPGenerator {
    fn header_template(&self) -> String;
    fn source_template(&self) -> String;
}

pub enum CPPModule<'a> {
    AppCanUtilsModule(AppCanUtilsModule<'a>),
    AppCanTxModule(AppCanTxModule<'a>),
    AppCanAlertsModule(AppCanAlertsModule<'a>),
    AppCanDataCaptureModule(AppCanDataCaptureModule<'a>),
    AppCanRxModule(AppCanRxModule<'a>),
    IoCanTxModule(IoCanTxModule<'a>),
    IoCanRxModule(IoCanRxModule<'a>),
    IoCanRerouteModule(IoCanRerouteModule<'a>),
}

impl CPPModule<'_> {
    pub fn header_template(&self) -> String {
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
    pub fn source_template(&self) -> String {
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
