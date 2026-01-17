rust my beloved


### overview

todo for backend
[x] reading from serial and parsing
[x] set up channels between handlers
[x] switch to using tokio (?)
[x] verify that serial messages can be processed
[ ] processing jsoncan to read metadata - deserialize bytes
[ ] implementing client subscription map
[ ] interfacing with influxdb and pushing data through
[ ] socket clients to publish data
[ ] proper error handling in backend and using optimal threading or whatnot
[ ] implementing api endpoints
[ ] implementing ntp and base time syncing
...

##### processing can signals
- main.rs creates queue between serial handler and influx handler
- serial_handler.rs reads packets and parses into can message, pushes to queue
- influx_handler.rs consumes and puts into influx db
- live_data_handler consumes and emits to clients via sockets

