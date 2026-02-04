rust my beloved


### overview

todo for backend
[x] reading from serial and parsing
[x] set up channels between handlers
[x] switch to using tokio threading
[x] verify that serial messages can be processed
[x] processing jsoncan to read metadata - deserialize bytes
[x] implementing client subscription map, socket server for clients to serve data
[ ] implementing api endpoints
[ ] figure out the time offset basetime thing, implement ntp and base time syncing
[ ] switch to tokio serial
[ ] set up mdns config
[ ] interfacing with influxdb and pushing data through
[ ] ensure proper error handling in backend and logging
...

##### processing can signals
- main.rs creates queue between serial handler and influx handler
- serial_handler.rs reads packets and parses into can message, pushes to queue
- influx_handler.rs consumes and puts into influx db
- live_data_handler consumes and emits to clients via sockets

