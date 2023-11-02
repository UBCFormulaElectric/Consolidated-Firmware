# Alerts JSON Schema

The `ECU_alerts.json` file is used to list the alerts sent by a specific ECU.
There are 2 types of alerts: Faults and warnings.
Faults will cause the DCM to exit drive state,
warnings will be displayed to the user via the DIM but no response will occur.

Specify the `warnings` and `faults` fields, and provide a list of all the alerts which the ECU sends.

In total, 4 messages will be generated: 

- `ECU_Warnings`: Indicates the present state of warnings.
- `ECU_WarningsCounts`: Indicate the number of times a warning was set since power on (counts not set -> set transitions).
- `ECU_Faults`: Indicates the present state of faults.
- `ECU_FaultsCounts`: Indicate the number of times a fault was set since power on (counts not set -> set transitions).

Specify their CAN message IDs using the `warnings_id`, `warnings_counts_id`, `faults_id`, and `faults_counts_id` fields.

Example: 

```json
{
    "warnings_id": 350,
    "warnings_counts_id": 351,
    "faults_id": 352,
    "faults_counts_id": 353,
    "warnings": [
        "Warning_LeftWheelSpeedOutOfRange",
        "Warning_RightWheelSpeedOutOfRange",
        "Warning_SteeringAngleOutOfRange",
    ],
    "faults": [
        "Fault_MissingHeartbeat",
        "Fault_StateMachine",
    ]
}
```