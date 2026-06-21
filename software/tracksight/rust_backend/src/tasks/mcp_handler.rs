use std::sync::Arc;

use jsoncan_rust::can_database::CanDatabase;
use rmcp::transport::streamable_http_server::{
    StreamableHttpServerConfig, StreamableHttpService,
    session::local::LocalSessionManager,
};

use crate::tasks::mcp::McpServer;

pub fn get_mcp_service(
    can_db: Arc<CanDatabase>,
    influx_client: Arc<influxdb2::Client>,
) -> StreamableHttpService<McpServer, LocalSessionManager> {
    let config = StreamableHttpServerConfig::default().disable_allowed_hosts();

    StreamableHttpService::new(
        move || Ok(McpServer::new(can_db.clone(), influx_client.clone())),
        Arc::new(LocalSessionManager::default()),
        config,
    )
}
