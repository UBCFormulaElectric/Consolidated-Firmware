use std::{collections::HashMap, sync::Arc, time::Duration};

use tokio::{select, time::sleep};

use rmcp::{
    ErrorData as McpError, ServerHandler,
    handler::server::wrapper::Parameters,
    model::{CallToolResult, Content, ServerCapabilities, ServerInfo},
    schemars, tool, tool_handler, tool_router,
};

use jsoncan_rust::can_database::{CanDatabase, CanMessage};

use crate::error_println;
use crate::tasks::client_api::INFLUX_QUERY_TIMEOUT_MS;
use crate::tasks::{
    can_data::{influx_util::InfluxSignalSource, sessions::get_sessions},
    client_api::signal_api_handler::{SignalMetadata, SignalMetadataEnumSignal},
};
use crate::{config::CONFIG, tasks::client_api::signal_tile::InfluxSignalRow};

#[derive(Clone)]
pub struct McpServer {
    pub can_db: Arc<CanDatabase>,
    pub influx_client: Arc<influxdb2::Client>,
}

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct ListSessionParams {
    pub start_utc: String,
    pub end_utc: String,
    pub source: String,
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

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct QueryRawParams {
    pub signal_names: Vec<String>,

    pub start_utc: String,
    pub end_utc: String,

    pub source: String,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct QueryRawResult {
    pub data_points: Vec<InfluxSignalRow>,
}

#[derive(Debug, serde::Deserialize, schemars::JsonSchema)]
pub struct QueryAggregateParams {
    pub signal_names: Vec<String>,

    pub start_utc: String,
    pub end_utc: String,

    pub source: String,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct SignalAggregate {
    pub signal_name: String,
    pub count: u64,
    pub min: f64,
    pub max: f64,
    pub mean: f64,
}

#[derive(Debug, serde::Serialize, schemars::JsonSchema)]
pub struct QueryAggregateResult {
    pub aggregates: Vec<SignalAggregate>,
}

fn parse_source(source: &str) -> Result<InfluxSignalSource, McpError> {
    source.parse::<InfluxSignalSource>().map_err(|_| {
        error_println!("[mcp] invalid source {:?}", source);

        McpError::invalid_params(
            format!(
                "Invalid source {:?}; expected \"Radio\" or \"SdCard\"",
                source
            ),
            None,
        )
    })
}

#[derive(Debug, influxdb2::FromDataPoint, Default)]
struct AggregateRow {
    signal_name: String,
    count: f64,
    min: f64,
    max: f64,
    mean: f64,
}

#[tool_router]
impl McpServer {
    pub fn new(can_db: Arc<CanDatabase>, influx_client: Arc<influxdb2::Client>) -> Self {
        Self {
            can_db,
            influx_client,
        }
    }

    #[tool(
        description = "Query raw telemetry data points for given signals over a time range. Source can be 'Radio' or 'SdCard'."
    )]
    async fn query_raw(
        &self,
        Parameters(QueryRawParams {
            signal_names,
            start_utc,
            end_utc,
            source,
        }): Parameters<QueryRawParams>,
    ) -> Result<CallToolResult, McpError> {
        let source = parse_source(&source)?;

        if signal_names.is_empty() {
            error_println!("[mcp] query_raw: signal_names was empty");

            return Err(McpError::invalid_params(
                "signal_names must contain at least one signal",
                None,
            ));
        }

        let source_str = source.to_string();

        let signal_set = signal_names
            .iter()
            .map(|name| format!("\"{}\"", name.replace('"', "\\\"")))
            .collect::<Vec<_>>()
            .join(", ");

        let query = format!(
            r#"
            from(bucket: "{bucket}")
                |> range(start: time(v: "{start_utc}"), stop: time(v: "{end_utc}"))
                |> filter(fn: (r) => r["_measurement"] == "{measurement}")
                |> filter(fn: (r) => r["source"] == "{source_str}")
                |> filter(fn: (r) => contains(value: r["signal_name"], set: [{signal_set}]))
                |> toFloat()
            "#,
            bucket = &CONFIG.influxdb_bucket,
            measurement = &CONFIG.influxdb_measurement,
        );

        let req: Result<Vec<InfluxSignalRow>, influxdb2::RequestError> = select! {
            val = self.influx_client.query::<InfluxSignalRow>(
                Some(influxdb2::models::Query::new(query))
            ) => val,
            _ = sleep(Duration::from_millis(INFLUX_QUERY_TIMEOUT_MS)) => {
                error_println!("[mcp] query_raw: InfluxDB query timed out after {}ms", INFLUX_QUERY_TIMEOUT_MS);
                return Err(McpError::internal_error(
                    "InfluxDB query timed out, try a smaller time range",
                    None,
                ));
            }
        };

        let data_points = req.map_err(|e| {
            error_println!("[mcp] query_raw: InfluxDB query failed: {}", e);
            McpError::internal_error("Failed to query InfluxDB", None)
        })?;

        let result = QueryRawResult { data_points };
        let serialized = serde_json::to_string(&result).map_err(|e| {
            error_println!("[mcp] query_raw: failed to serialize result: {}", e);
            McpError::internal_error(format!("Failed to serialize result: {}", e), None)
        })?;

        Ok(CallToolResult::success(vec![Content::text(serialized)]))
    }

    #[tool(
        description = "Query aggregated telemetry data for given signals over a time range. Source can be 'Radio' or 'SdCard'."
    )]
    async fn query_aggregate(
        &self,
        Parameters(QueryAggregateParams {
            signal_names,
            start_utc,
            end_utc,
            source,
        }): Parameters<QueryAggregateParams>,
    ) -> Result<CallToolResult, McpError> {
        let source = parse_source(&source)?;

        if signal_names.is_empty() {
            error_println!("[mcp] query_aggregate: signal_names was empty");
            return Err(McpError::invalid_params(
                "signal_names must contain at least one signal",
                None,
            ));
        }

        let source_str = source.to_string();

        let signal_set = signal_names
            .iter()
            .map(|name| format!("\"{}\"", name.replace('"', "\\\"")))
            .collect::<Vec<_>>()
            .join(", ");

        let query = format!(
            r#"
            base = from(bucket: "{bucket}")
                |> range(start: time(v: "{start_utc}"), stop: time(v: "{end_utc}"))
                |> filter(fn: (r) => r["_measurement"] == "{measurement}")
                |> filter(fn: (r) => r["source"] == "{source_str}")
                |> filter(fn: (r) => contains(value: r["signal_name"], set: [{signal_set}]))
                |> group(columns: ["signal_name"])

            union(tables: [
                base |> count() |> map(fn: (r) => ({{ signal_name: r.signal_name, _field: "count", _value: float(v: r._value) }})),
                base |> min()   |> map(fn: (r) => ({{ signal_name: r.signal_name, _field: "min",   _value: r._value }})),
                base |> max()   |> map(fn: (r) => ({{ signal_name: r.signal_name, _field: "max",   _value: r._value }})),
                base |> mean()  |> map(fn: (r) => ({{ signal_name: r.signal_name, _field: "mean",  _value: r._value }})),
            ])
            "#,
            bucket = &CONFIG.influxdb_bucket,
            measurement = &CONFIG.influxdb_measurement,
        );

        let req: Result<Vec<AggregateRow>, influxdb2::RequestError> = select! {
            val = self.influx_client.query::<AggregateRow>(
                Some(influxdb2::models::Query::new(query))
            ) => val,
            _ = sleep(Duration::from_millis(INFLUX_QUERY_TIMEOUT_MS)) => {
                error_println!("[mcp] query_aggregate: InfluxDB query timed out after {}ms", INFLUX_QUERY_TIMEOUT_MS);
                return Err(McpError::internal_error(
                    "InfluxDB query timed out, try a smaller time range",
                    None,
                ));
            }
        };

        let rows = req.map_err(|e| {
            error_println!(
                "[mcp] query_aggregate: failed to query aggregate data: {}",
                e
            );
            McpError::internal_error(format!("Failed to query aggregate data: {}", e), None)
        })?;

        let aggregates = rows
            .into_iter()
            .map(|row| SignalAggregate {
                signal_name: row.signal_name,
                count: row.count as u64,
                min: row.min,
                max: row.max,
                mean: row.mean,
            })
            .collect::<Vec<_>>();

        let result = QueryAggregateResult { aggregates };
        let serialized = serde_json::to_string(&result).map_err(|e| {
            error_println!("[mcp] query_aggregate: failed to serialize result: {}", e);
            McpError::internal_error(format!("Failed to serialize result: {}", e), None)
        })?;

        Ok(CallToolResult::success(vec![Content::text(serialized)]))
    }

    #[tool(description = "Get metadata for multiple CAN bus signals by name.")]
    async fn get_signals_metadata(
        &self,
        Parameters(SignalNameParam {
            names: signal_names,
        }): Parameters<SignalNameParam>,
    ) -> Result<CallToolResult, McpError> {
        let flat_map = |msg: &CanMessage| {
            return msg
                .signals
                .iter()
                .map(|signal| {
                    let can_enum: Option<SignalMetadataEnumSignal> = if let Some(enum_name) =
                        &signal.enum_name
                        && let Some(can_enum) = self.can_db.get_enum(enum_name)
                    {
                        Some(SignalMetadataEnumSignal {
                            enum_name: can_enum.name.clone(),
                            enum_values: can_enum.values.clone(),
                        })
                    } else {
                        None
                    };

                    (
                        signal.name.clone(),
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
                        },
                    )
                })
                .collect::<Vec<_>>();
        };

        let metadatas: HashMap<String, SignalMetadata> = self
            .can_db
            .get_all_msgs()
            .unwrap_or_default()
            .iter()
            .filter(|msg| {
                msg.signals
                    .iter()
                    .any(|signal| signal_names.contains(&signal.name))
            })
            .flat_map(flat_map)
            .filter(|(name, _)| signal_names.contains(name))
            .collect();

        return Ok(CallToolResult::success(vec![Content::text(
            serde_json::to_string(&metadatas).map_err(|e| {
                error_println!(
                    "[mcp] get_signals_metadata: failed to serialize result: {}",
                    e
                );
                McpError::internal_error(format!("Failed to serialize result: {}", e), None)
            })?,
        )]));
    }

    #[tool(description = "List CAN bus signals for a given node.")]
    async fn list_signals(
        &self,
        Parameters(ListSignalsParams {
            node_name,
            page,
            page_size,
        }): Parameters<ListSignalsParams>,
    ) -> Result<CallToolResult, McpError> {
        let node = self.can_db.nodes.iter().find(|n| n.name == node_name);

        if node.is_none() {
            error_println!(
                "[mcp] list_signals: node '{}' not found in CAN database",
                node_name
            );
            return Err(McpError::internal_error(
                format!("Node '{}' not found in CAN database", node_name),
                None,
            ));
        }

        let page_size = page_size.unwrap_or(50);
        if page_size == 0 {
            error_println!("[mcp] list_signals: page_size must be greater than 0");
            return Err(McpError::invalid_params(
                "page_size must be greater than 0",
                None,
            ));
        }

        let page = page.unwrap_or(0);

        let all_signals = self
            .can_db
            .get_all_msgs()
            .into_iter()
            .flatten()
            .filter(|msg| msg.tx_node_name == node_name)
            .flat_map(|msg| msg.signals.into_iter().map(|signal| signal.name.clone()))
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
            error_println!("[mcp] list_signals: failed to serialize result: {}", e);
            McpError::internal_error(format!("Failed to serialize result: {}", e), None)
        })?;

        Ok(CallToolResult::success(vec![Content::text(serialized)]))
    }

    #[tool(description = "List CAN bus signal nodes available in the database.")]
    async fn list_tx_nodes(&self) -> Result<CallToolResult, McpError> {
        let nodes = self
            .can_db
            .nodes
            .iter()
            .map(|node| node.name.clone())
            .collect::<Vec<_>>();

        let result = ListTxNodesResult { nodes };
        let serialized = serde_json::to_string(&result);

        if let Err(e) = serialized {
            error_println!("[mcp] list_tx_nodes: failed to serialize result: {}", e);
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

    #[tool(description = "List historical telemetry sessions. Source can be 'Radio' or 'SdCard'.")]
    async fn list_sessions(
        &self,
        Parameters(ListSessionParams {
            start_utc,
            end_utc,
            source,
        }): Parameters<ListSessionParams>,
    ) -> Result<CallToolResult, McpError> {
        let source = parse_source(&source)?;

        let source_str = match source {
            InfluxSignalSource::Radio => "radio",
            InfluxSignalSource::SdCard => "sdcard",
        };

        let sessions_res = get_sessions(
            start_utc,
            end_utc,
            source_str.to_string(),
            self.influx_client.clone(),
        )
        .await;

        match sessions_res {
            Ok(sessions) => {
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
                            error_println!(
                                "[mcp] list_sessions: failed to parse session start time {:?}: {}",
                                s.0,
                                e
                            );

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
                                error_println!(
                                    "[mcp] list_sessions: failed to parse session end time {:?}: {}",
                                    end_str,
                                    e
                                );

                                return Err(McpError::internal_error(
                                    format!(
                                        "Failed to parse session end time {:?}: {}",
                                        end_str, e
                                    ),
                                    None,
                                ));
                            }
                        },
                        None => None,
                    };

                    let duration_ms = end_ms
                        .map_or(chrono::Utc::now().timestamp_millis() - start_ms, |end| {
                            end - start_ms
                        });

                    parsed_sessions.push(SessionInfo {
                        start_time: to_rfc3339(start_ms),
                        end_time: end_ms.map(to_rfc3339),
                        duration_ms: duration_ms,
                        source: source.clone(),
                        open: end_ms.is_none(),
                    });
                }

                let result = ListSessionResult {
                    sessions: parsed_sessions,
                };
                let serialized = serde_json::to_string(&result);

                if let Err(e) = serialized {
                    error_println!("[mcp] list_sessions: failed to serialize result: {}", e);
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
            Err(e) => {
                error_println!("[mcp] list_sessions: failed to get sessions: {}", e);

                Err(McpError::internal_error(
                    format!("Failed to get sessions: {}", e),
                    None,
                ))
            }
        }
    }
}

#[tool_handler]
impl ServerHandler for McpServer {
    fn get_info(&self) -> ServerInfo {
        let mut info = ServerInfo::default();

        info.capabilities = ServerCapabilities::builder().enable_tools().build();

        info.instructions = Some(
            "UBC Formula Electric telemetry server. Query CAN signals from telemetry sessions recorded on the car.".to_string().into(),
        );

        return info;
    }
}
