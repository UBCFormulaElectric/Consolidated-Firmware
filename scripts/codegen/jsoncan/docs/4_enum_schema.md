# Enum JSON Schema

The `ECU_enum.json` file is used to list the enums used by a single ECU. Enums can be specified in either the ECU-specific JSON file or the `shared_enum.json` file.

Specify an enum like so:

```json
{
    "ExampleEnum": {
        "OPTION_1": 0,
        "OPTION_2": 1,
        "OPTION_3": 2
    }
}
```

The enum values will be taken to be the values specified without any scaling, so I recommend to closely-pack them. An option with value 0 is required.