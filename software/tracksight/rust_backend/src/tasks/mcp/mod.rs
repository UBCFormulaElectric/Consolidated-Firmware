use std::{collections::HashMap, sync::Arc};

use rmcp::{
    ErrorData as McpError, RoleServer, ServerHandler, handler::server::{router::tool::ToolRouter, wrapper::Parameters}, model::{AnnotateAble, Annotations, CallToolResult, Content, ListResourcesResult, PaginatedRequestParams, RawResource, Role, ServerCapabilities, ServerInfo}, schemars, service::RequestContext, tool, tool_handler, tool_router
};

use jsoncan_rust::can_database::{CanDatabase, CanMessage};

use crate::tasks::{can_data::{influx_util::InfluxSignalSource, sessions::get_sessions}, client_api::signal_api_handler::{SignalMetadata, SignalMetadataEnumSignal}};

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

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct ListSignalsParams {
    pub node_name: String,
    pub page: Option<usize>,
    pub page_size: Option<usize>,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct ListSignalsResult {
    pub signals: Vec<String>,
    
    pub page: usize,
    pub page_size: usize,
    pub total: usize,
    pub total_pages: usize,
    pub has_next_page: bool,
}

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct SignalNameParam {
    pub names: Vec<String>,
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

    #[tool(description = "Get metadata for multiple CAN bus signals by name.")]
    async fn get_signals_metadata(&self, Parameters(SignalNameParam { names: signal_names }): Parameters<SignalNameParam>) -> Result<CallToolResult, McpError> {
        let flat_map = | msg: &CanMessage | {
            return msg.signals.iter().map(
                | signal | {
                    let can_enum: Option<SignalMetadataEnumSignal> = 
                        if let Some(enum_name) = &signal.enum_name && 
                            let Some(can_enum) = 
                                self.can_db.get_enum(enum_name) {
                                    Some(SignalMetadataEnumSignal {
                                        enum_name: can_enum.name.clone(),
                                        enum_values: can_enum.values.clone()
                                    })
                                } else {
                                    None
                                };
                                
                    (signal.name.clone(),
                        SignalMetadata {
                            name: signal.name.clone(),
                            min_val: signal.min,
                            max_val: signal.max,
                            unit: signal.unit.clone(),
                            enum_signal: can_enum,
                            tx_node: msg.tx_node_name.clone(),
                            cycle_time_ms: msg.cycle_time.clone(),
                            id: msg.id,
                            msg_name: msg.name.clone(),
                        })
                }
            ).collect::<Vec<_>>()
        };

        let metadatas: HashMap<String, SignalMetadata> = self.can_db.get_all_msgs()
            .unwrap_or_default()
            .iter()
            .filter(|msg| msg.signals.iter().any(|signal| signal_names.contains(&signal.name)))
            .flat_map(flat_map)
            .filter(|(name, _)| signal_names.contains(name))
            .collect();

        return Ok(CallToolResult::success(vec![Content::text(
            serde_json::to_string(&metadatas).map_err(|e| {
                McpError::internal_error(format!("Failed to serialize result: {}", e), None)
            })?
        )]));
    }

    #[tool(description = "List CAN bus signals for a given node.")]
    async fn list_signals(
        &self,
        Parameters(ListSignalsParams { node_name, page, page_size }): Parameters<ListSignalsParams>,
    ) -> Result<CallToolResult, McpError> {
        let node = self.can_db.nodes.iter().find(|n| n.name == node_name);

        if node.is_none() {
            return Err(McpError::internal_error(
                format!("Node '{}' not found in CAN database", node_name),
                None,
            ));
        }

        let page_size = page_size.unwrap_or(50);
        if page_size == 0 {
            return Err(McpError::invalid_params(
                "page_size must be greater than 0",
                None,
            ));
        }

        let page = page.unwrap_or(0);

        let all_signals = self.can_db
            .get_all_msgs()
            .into_iter()
            .flatten()
            .filter(|msg| { msg.tx_node_name == node_name })
            .flat_map(|msg| { msg.signals.into_iter().map(|signal| signal.name.clone()) })
            .collect::<Vec<_>>();

        let total = all_signals.len();
        let total_pages = total.div_ceil(page_size);

        let signals = all_signals
            .into_iter()
            .skip(page * page_size)
            .take(page_size)
            .collect::<Vec<_>>();

        let result = ListSignalsResult {
            signals,
            page,
            page_size,
            total,
            total_pages,
            has_next_page: page + 1 < total_pages,
        };
        let serialized = serde_json::to_string(&result).map_err(|e| {
            McpError::internal_error(format!("Failed to serialize result: {}", e), None)
        })?;

        Ok(CallToolResult::success(vec![Content::text(serialized)]))
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
