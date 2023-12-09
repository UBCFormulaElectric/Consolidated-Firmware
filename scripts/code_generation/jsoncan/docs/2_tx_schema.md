# TX JSON Schema

## Overview

The `ECU_tx.json` file is used to describe the messages, and the signals that make up the messages, for a single ECU.

## Defining a Message
Define a  message as below:

```json
{
    "message_name": {
        "msg_id": 100, 
        "cycle_time": 1000,
        "allowed_modes": ["default", "debug"],
        "signals": {
            "signal_name_1": {
                "bits": 1,
            },
            "signal_name_2": {
                "bits": 3,
            },
        }
    },
}
```

If `cycle_time` is not specified, the message will be aperiodic and you will have to transmit the message manually. 

`allowed_modes` defines which modes the message will be transmitted in. You can enable/disable the different modes to choose which messages are being sent at runtime.
If `allowed_modes` is not included, then the message will only be sent if the default mode is enabled.

## Defining a Signal

There are currently 5 supported ways to define a signal's value and representation.

### 1. Specify Bits
If signed is false, the signal will be encoded as an unsigned integer with the amount of bits specified. The signal values can range from 0 to `2^N-1`, where N is the number of bits.

If signed is true, the signal will be encoded as a signed integer, ranging from `-2^(N-1)` to `2^(N-1)-1`. 

Signed is optional, and is assumed to be false. This is useful for implementing simple whole-number counters.

```json
"signal_name": {
    "bits": 5,
    "signed": false
}
```

### 2. Specify Bits, Min, Max

Signals can take values ranging from `min` to `max`, with an encoded bit length specified by `bits`. The signal is encoded as an unsigned integer, where the maximum value (`2^N-1`) cooresponds to `max`, and 0 cooresponds to `min`. The precision is calculated accordingly.

```json
"signal_name": {
    "bits": 5,
    "min": 0,
    "max": 100,
}
```

### 3. Specify Resolution, Min, Max

Signals can take values ranging from `min` to `max`, with a precision specified by `resolution`. For example, in the JSON below `signal_name` can take values ranging from 0 to 10 to the nearest 1 decimal place. 

The number of bits needed to encode this signal is given by `ceil(log_2((max-min)/resolution)`. For example, the signal below can take a total of `(10-0)/0.1 = 100` possible values requiring 7 bits. The signal is encoded as an unsigned integer, so `2^7 = 128` is the tighest upper bound which can take all values.

```json
"signal_name": {
    "resolution": 0.1,
    "min": 0,
    "max": 10,
}
```

### 4. Specify an Enum

If an enum is specified, the enum's name will be searched for in the ECU-specific `ECU_enum.json` and the `shared_enum.json` files. The bit length of the signal will be enough to hold the enum's maximum value as an unsigned integer. For example, the enum below has a maximum value of 2, and so is 2 bits long.

In the `ECU_tx.json`: 

```json
"signal_name": {
    "enum": "ExampleEnum",
}
```

In the `_enum.json` (either shared or ECU-specific):

```json
"ExampleEnum": {
    "OPTION_1": 0,
    "OPTION_2": 1,
    "OPTION_3": 2
}
```

### 5. Specify All DBC Parameters

Alternatively, you can specify a signal with the usual DBC parameters (scale, offset, min, max, bit length). This is only necessary when describing signals with a signed representation, such as those transmitted by the inverters (signed is optional, assumed to be false).

```json
"signal_name": {
    "scale": 1,
    "offset": 0,
    "bits": 5,
    "min": 0,
    "max": 100,
    "signed": false 
}
```

## Specifying Signal Position

Signals will be tightly-packed unless otherwise specified, one after another, in the order they are specified.

Use the optional `start_bit` field to override this behavior:

```json
"signal_name": {
    "bits": 3,
    "start_bit": 5 
}
```

Message length will be the minimum number of bytes required to hold all signals, up to a maximum of 8.

## Attributes

### Start Value

Use the optional `start_value` field to set the `GenSigStartValue` attribute.

```json
"signal_name": {
    "bits": 3,
    "start_val": 99
}
``` 

### Units

Use the optional `unit` field to define the units for a signal, as long as the signal doesn't use an enum.

```json
"signal_name": {
    "bits": 3,
    "unit": "m/s"
}
``` 

## Full Example

The following `FSM_tx.json` file...

```json
{
    "FSM_Sensors": {
        "msg_id": 400,
        "cycle_time": 1000,
        "signals": {
            "BrakeIsAcuated": { 
                "bits": 1 
            },
            "BrakePressure": {
                "min": 0, 
                "max": 2500,
                "bits": 10, 
                "unit": "psi"        
            },
            "SteeringAngle": {
                "min": -120, 
                "max": 120, 
                "resolution": 1 
            },
            "State": {
                "enum": "FsmState"
            },
            "PappsPercentage": {
                "min": 0,
                "max": 100, 
                "scale": 1,
                "offset": 0, 
                "bits": 7,
                "signed": true,
                "unit": "%"
            }
        }
    },
}
```

...would generate the following DBC:

```dbc
BO_ 400 FSM_Sensors: 4 FSM
SG_ BrakeIsAcuated : 0|1@1+ (1,0) [0|1] "" DEBUG
SG_ BrakePressure : 1|10@1+ (2.4437927663734116,0) [0|2500] "psi" DEBUG
SG_ SteeringAngle : 11|8@1+ (1,-120) [-120|120] "" DEBUG
SG_ State : 19|1@1+ (1,0) [0|1] "" DEBUG
SG_ PappsPercentage : 20|7@1- (1,0) [0|100] "%" DEBUG
```

NOTE: This is just an example and does not follow best practices. For example, there is no reason for `PappsPercentage` to be signed.
