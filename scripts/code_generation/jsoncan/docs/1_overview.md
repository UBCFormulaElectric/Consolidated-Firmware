# Schema Overview

These next few files provide documentation on the schema used by JSON CAN to configure CAN messages.

The file structure looks like: 

```
bus/
- bus.json
- shared_enum.json
- ecu_1/
    - ecu_1_tx.json
    - ecu_1_rx.json
    - ecu_1_enum.json
    - ecu_1_alerts.json
- ecu_2/
    - ecu_2_tx.json
    - ecu_2_rx.json
    - ecu_2_enum.json
    - ecu_2_alerts.json
```

## Shared Files

- `bus.json` contains bus-specific parameters. It includes the list of available "modes". Only messages allowed for the currently enabled modes will be transmitted on the bus.
- `shared_enum.json` lists enums used by multiple ECUs.

## ECU-Specific Files

Each ECU defines their own version of the following files, in a subfolder with the name ECU.

- `ECU_tx.json`: Describe the messages transmitted by ECU, and the signals that make up the messages, for a single ECU.
- `ECU_rx.json`: List the messages received by ECU.
- `ECU_enum.json`: List the enums that ECU uses in its signals.
- `ECU_alerts.json`: Alerts are WIP.

## Generated Code

The generated code for a single ECU includes functionality to send all the messages described in `ECU_tx.json`, and receive all the messages listed in `ECU_rx.json`. 
