# CanMsgs
This folder contains everything to do with the CAN messages that we send between controllers on the car. 

## Contents
 - `CanMsgs.sym`: defines all the CAN messages on the car
 - `generate_c_code_from_sym.py`: this script generates `CanMsgs.c` and `CanMsgs.h` from `CanMsgs.sym`, allowing us a C interface to all the CAN messages

 ## Making Changes to CAN Messages
 1. Edit the `.sym` using `PCAN-View` (which is fre to download)
 1. Run `generate_c_code_from_sym.py` to generate the update `CanMsgs.c` and `CanMsgs.h` files.
