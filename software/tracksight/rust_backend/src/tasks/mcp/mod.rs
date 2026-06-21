use std::sync::Arc;

use rmcp::{
    ErrorData as McpError, RoleServer, ServerHandler, handler::server::{router::tool::ToolRouter, wrapper::Parameters}, model::{AnnotateAble, Annotations, CallToolResult, Content, ListResourcesResult, PaginatedRequestParams, RawResource, Role, ServerCapabilities, ServerInfo}, schemars, service::RequestContext, tool, tool_handler, tool_router
};

use jsoncan_rust::can_database::CanDatabase;

use crate::tasks::can_data::{influx_util::InfluxSignalSource, sessions::get_sessions};

#[derive(Clone)]
pub struct McpServer {
    pub can_db: Arc<CanDatabase>,
    pub influx_client: Arc<influxdb2::Client>,

    tool_router: ToolRouter<Self>,
}

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct ListSessionParams {
    pub start_utc: String,
    pub end_utc: String,
    pub source: InfluxSignalSource,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct SessionInfo {
    pub start_time: String,
    pub end_time: Option<String>,
    pub duration_ms: i64,
    pub source: InfluxSignalSource,
    pub open: bool,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct ListSessionResult {
    pub sessions: Vec<SessionInfo>,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct ListTxNodesResult {
    pub nodes: Vec<String>,
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

    #[tool(description = "List CAN bus signal nodes available in the database.")]
    async fn list_tx_nodes(&self) -> Result<CallToolResult, McpError> {
        let nodes = self.can_db.nodes
            .iter().map(|node| node.name.clone())
            .collect::<Vec<_>>();

        let result = ListTxNodesResult { nodes };
        let serialized = serde_json::to_string(&result);

        if let Err(e) = serialized {
            return Err(McpError::internal_error(
                format!("Failed to serialize result: {}", e),
                None,
            ));
        }

        Ok(CallToolResult::success(vec![Content::text(format!(
            "{}", 
            serialized.unwrap()
        ))]))
    }

    #[tool(description = "List historical telemetry sessions.")]
    async fn list_sessions(
        &self,
        Parameters(ListSessionParams { start_utc, end_utc, source }): Parameters<ListSessionParams>,
    ) -> Result<CallToolResult, McpError> {
        let source_str = match source {
            InfluxSignalSource::Radio => "radio",
            InfluxSignalSource::SdCard => "sdcard"
        };  

        let sessions_res = get_sessions(
            start_utc, 
            end_utc, 
            source_str.to_string(), 
            self.influx_client.clone(),
        ).await;
        
        match sessions_res {
            Ok(sessions) =>{
                // `get_sessions` returns unix-millis-as-string, not RFC3339.
                let to_rfc3339 = |ms: i64| {
                    chrono::DateTime::<chrono::Utc>::from_timestamp_millis(ms)
                        .map(|dt| dt.to_rfc3339())
                        .unwrap_or_else(|| ms.to_string())
                };

                let mut parsed_sessions: Vec<SessionInfo> = Vec::with_capacity(sessions.len());
                for s in sessions {
                    let start_ms = match s.0.parse::<i64>() {
                        Ok(ms) => ms,
                        Err(e) => {
                            return Err(McpError::internal_error(
                                format!("Failed to parse session start time {:?}: {}", s.0, e),
                                None,
                            ));
                        }
                    };
                    
                    let end_ms = match &s.1 {
                        Some(end_str) => match end_str.parse::<i64>() {
                            Ok(ms) => Some(ms),
                            Err(e) => {
                                return Err(McpError::internal_error(
                                    format!("Failed to parse session end time {:?}: {}", end_str, e),
                                    None,
                                ));
                            }
                        },
                        None => None,
                    };

                    let duration_ms = end_ms.map_or(chrono::Utc::now().timestamp_millis() - start_ms, |end| end - start_ms);

                    parsed_sessions.push(SessionInfo {
                        start_time: to_rfc3339(start_ms),
                        end_time: end_ms.map(to_rfc3339),
                        duration_ms: duration_ms,
                        source: source.clone(),
                        open: end_ms.is_none(),
                    });
                }

                let result = ListSessionResult { sessions: parsed_sessions };
                let serialized = serde_json::to_string(&result);

                if let Err(e) = serialized {
                    return Err(McpError::internal_error(
                        format!("Failed to serialize result: {}", e),
                        None,
                    ));
                }

                Ok(CallToolResult::success(vec![Content::text(format!(
                    "{}", 
                    serialized.unwrap()
                ))]))              
            }
            Err(e) => Err(McpError::internal_error(
                format!("Failed to get sessions: {}", e),
                None,
            )),
        }
    }
}

#[tool_handler]
impl ServerHandler for McpServer {
    fn get_info(&self) -> ServerInfo {
        let mut info = ServerInfo::default();

        info.capabilities = ServerCapabilities::builder()
            .enable_tools()
            .build();

        info.instructions = Some(
            "UBC Formula Electric telemetry server. Query CAN signals from telemetry sessions recorded on the car.".to_string().into(),
        );

        return info;
    }
}
