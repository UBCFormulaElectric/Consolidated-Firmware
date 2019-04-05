# Module Specifications

This folder contains the specifications for each module. The intent is to have each file act as a definitive resource that fully defines all board behaviors, as observed from outside the board. 
Each specification has an unique ID, short title, and detailed description. It may also be associted with a competition rule.

## Rules
1. Specification ID's should never be duplicated. 
1. Specification ID's should only increment. This means that even if have 50 FSM specs and we delete `FSM-23`, the next spec should be `FSM-51`, we should never re-use ID's.
1. Specification descriptions should be unambiguous and formal. Instead of "acceptable value" say "5 V". Instead of "shall send out error constantly", say "shall send out the CAN message `ERR_CAN_MSG` every 4 seconds until the board is reset".

## Table Of Contents
- [FSM (Front Sensor Module)](#FSM)

## FSM
ID | Title | Description | Rule
--- | --- | ---
FSM-0 | Brake OverTravel Switch Triggered | When the voltage on pin A drops below 2.5V, the FSM shall constantly send the `FSM_AIR_SHUTDOWN` CAN signal with a period of 5 seconds until the board is reset. |
