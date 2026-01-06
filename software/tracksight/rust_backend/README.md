rust my beloved


### overview

todo for backend
[x] reading from serial and parsing
[x] queuing can messages to push to influx
[ ] verify that serial messages can be processed
[ ] switch to using tokio (?)
[ ] interfacing with influxdb and pushing data through
[ ] processing jsoncan to read metadata
[ ] socket clients to publish data
[ ] proper error handling in backend and using optimal threading or whatnot
...

##### processing can signals
- main.rs creates queue between serial handler and influx handler
- serial_handler.rs reads packets and parses into can message, pushes to queue
- influx_handler.rs pops from queue, puts into influx db

