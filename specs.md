# Module Specifications

This folder contains the specifications for each module. The intent is to have each file act as a definitive resource that fully defines all board behaviors, as observed from outside the board. 
Each specification has an unique ID, short title, and detailed description. It may also be associted with a competition rule.

## Specification Rules
1. Specification ID's should never be duplicated. 
1. Specification ID's should only increment. This means that even if have 50 FSM specs and we delete `FSM-23`, the next spec should be `FSM-51`, we should never re-use ID's.
1. Specification descriptions should be unambiguous and formal. Instead of "acceptable value" say "5 V". Instead of "shall send out error constantly", say "shall send out the CAN message `ERR_CAN_MSG` every 4 seconds until the board is reset".
1. All abbreviations (`AIR`, `APPS`, etc.) should be spelled out fully the first time they appear in a given row in the table. This means that if `APPS` appears in `FSM-0` and `FSM-22`, it should be spelled out as `APPS (acceleration pedal position sensor)` in _both_ `FSM-0` and `FSM-22`. 

## Table Of Contents
- [FSM (Front Sensor Module)](#FSM)

## FSM
ID | Title | Description | Associated Competition Rule(s)
--- | --- | --- | ---
FSM-0 | APPS (Accelerator Pedal Position Sensor) Disagreement | When the ADC readings between the primary and secondary APP's disagree by more then **(TODO: what adc value is equivalent to 10% pedal travel?)**, the FSM shall constantly send the `FSM_AIR_SHUTDOWN` CAN signal with a period of 5 seconds until the board is reset. | T.6.2.3, T.6.2.4
