# Conventions

## Naming Conventions

1. Names should be descriptive and avoid using any uncommon or undocumented acronyms.
2. Message names are in `PascalCase`, prefixed by the board name in caps. For example, `BMS_HighVoltage`. 
3. Signal and enum names are in `PascalCase`. 
4. Enum values are in `SCREAMING_SNAKE_CASE`. 
5. Alert names are in `SCREAMING_SNAKE_CASE`, and prefixed with `<board name>_ALERT`. For example, `BMS_ALERT_ACCUMULATOR_OVERTEMP`.

## Message IDs

Standard CAN allows an 11-bit message identifier, so up to 2048 possible IDs. High priority messages will have IDs in [0, 999], low priority messages will have IDs in [1000, 1999]. Each board will be assigned a range of 100s as follows:

1. 0s range: Inverters
2. 100s range: BMS
3. 200s range: DCM
4. 300s range: FSM
5. 400s range: PDM
6. 500s range: DIM
7. 600s range: GSM
8. 700s range: Debug messages

For example, high priority BMS messages would have IDs from 100-199, and low priority from 1100-1199.

## Misc. Conventions

1. All signals representing a physical value will include a unit, such as `m/s`, `degC`, `rad`, etc.