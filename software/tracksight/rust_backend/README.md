rust my beloved


### overview

todo for backend
[x] reading from serial and parsing
[x] queuing can messages to push to influx
[ ] interfacing with influxdb and pushing data through
[ ] processing jsoncan to read metadata
[ ] socket clients to publish data
...

##### processing can signals
- main.rs creates queue between serial handler and influx handler
- serial_handler.rs reads packets and parses into can message, pushes to queue
- influx_handler.rs pops from queue, puts into influx db

