### rust my beloved

for backend todos, see jira ticket [data-70](https://ubcformulaelectric.atlassian.net/browse/DATA-70)

### overview
- `main.rs` creates and dispatches a bunch of tokio tasks
- `serial_handler.rs` reads bytes from radio and parses into can message
- `can_data_handler` takes can messages and unpacks into decoded signals
    - `influx_handler.rs` consumes decoded signals and puts into influxdb
    - `live_data_handler.rs` consumes and emits to clients via websockets
- `api_handler.rs` runs axum api server for frontend

