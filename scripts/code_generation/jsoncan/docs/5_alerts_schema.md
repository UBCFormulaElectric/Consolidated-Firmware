# Alerts JSON Schema

The `ECU_alerts.json` file is used to list the alerts sent by a specific ECU.
There are 2 types of alerts: Faults and warnings.
Faults will cause the BMS to open contactors and enter fault state,
warnings will be displayed to the user via the DIM but no response will occur.

Specify the `warnings` and `faults` fields, and provide a list of all the alerts which the ECU sends.
Also specify the `warnings_id` and `faults_id` field, the ID of the generated CAN messages for the ECU's alerts.

Example: 

```json
{
    "warnings_id": 350,
    "faults_id": 351,
    "warnings": [
        "LeftWheelSpeedOutOfRangeWarning",
        "RightWheelSpeedOutOfRangeWarning",
        "SteeringAngleOutOfRangeWarning",
    ],
    "faults": [
        "MissingHeartbeatFault",
        "StateMachineFault",
    ]
}
```