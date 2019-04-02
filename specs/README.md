# Module Specifications

This folder contains the specifications for each module. The intent is to have each file act as a definitive resource that fully defines all board behaviors, as observed from outside the board.

## Rules
1. Specification ID's should never be duplicated. 
1. Specification ID's should only increment. This means that even if have 50 FSM specs and we delete `FSM-23`, the next spec should be `FSM-51`, we should never re-use ID's.
1. Specification descriptions should be unambiguous and formal. Instead of "acceptable value" say "5 V". Instead of "shall send out error constantly", say "shall send out the CAN message `ERR_CAN_MSG` every 4 seconds until the board is reset".
