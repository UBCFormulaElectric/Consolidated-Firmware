use std::sync::Arc;

use rmcp::{
    ErrorData as McpError, ServerHandler,
    handler::server::{router::tool::ToolRouter, wrapper::Parameters},
    model::{CallToolResult, Content, ServerCapabilities, ServerInfo},
    tool, tool_handler, tool_router,
};
use rmcp::schemars;
use serde::Deserialize;

use jsoncan_rust::can_database::CanDatabase;

#[derive(Clone)]
pub struct McpServer {
    pub can_db: Arc<CanDatabase>,
    pub influx_client: Arc<influxdb2::Client>,

    tool_router: ToolRouter<Self>,
}

#[tool_router]
impl McpServer {
    pub fn new(can_db: Arc<CanDatabase>, influx_client: Arc<influxdb2::Client>) -> Self {
        Self {
            can_db,
            influx_client,
            tool_router: Self::tool_router(),
        }
    }

    #[tool(description = "List available CAN signal names")]
    async fn list_signals(
        &self,
    ) -> Result<CallToolResult, McpError> {
        let _ = (&self.can_db, &self.influx_client);

        Ok(CallToolResult::success(vec![Content::text(
            "No signals found",
        )]))
    }
}

#[tool_handler]
impl ServerHandler for McpServer {
    fn get_info(&self) -> ServerInfo {
        let mut info = ServerInfo::default();

        info.capabilities = ServerCapabilities::builder().enable_tools().build();
        info.instructions = Some(
            "Tracksight telemetry backend. Query CAN signals and historical telemetry data."
                .into(),
        );

        return info;
    }
}
