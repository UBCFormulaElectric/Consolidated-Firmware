# Conventions

## Naming Conventions

1. Names should be descriptive and avoid using any uncommon or undocumented acronyms.
2. Message and signal names are in `PascalCase`. ECU name prefixes will be added automatically.
3. Enum names are in `PascalCase`
4. Enum values are in `SCREAMING_SNAKE_CASE`. 
5. Alert names are in `PascalCase`, ending in `Faults` or `Warnings` depending on the alert type. ECU name prefixes will be added automatically.

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