# RX JSON Schema

The `ECU_rx.json` file is used to list the messages recieved by a single ECU.

Specify the `messages` field, and provide a list of all the messages which the ECU wants to receive.

Example: 

```json
{
    "messages": [
        "FSM_Vitals",
        "FSM_Sensors",
        "BMS_Vitals",
        "BMS_State",
        "BMS_HvStatus"
    ]
}
```