# CanMsgs
This folder contains everything to do with the CAN messages that we send between controllers on the car. 

## Contents
 - `CanMsgs.sym`: defines all the CAN messages on the car
 - `generate_c_code_from_sym.py`: this script generates `CanMsgs.c` and `CanMsgs.h` from `CanMsgs.sym`, allowing us a C interface to all the CAN messages

 ## Making Changes to CAN Messages
 1. Edit the `.sym` using `PCAN-View` (which is fre to download)
 1. Run `generate_c_code_from_sym.py` to generate the update `CanMsgs.c` and `CanMsgs.h` files.

 ## Working With The Generated C Code
 For every CAN message (hereafter generally referred to as `MSG_NAME`), where will be:
 - `CanMsgs_MSG_NAME_t`: a struct that is setup to hold the various elements of the message
 - `CanMsgs_MSG_NAME_pack`: a function to pack the `CanMsgs_MSG_NAME_t` struct into a data array that we can transmit over CAN  
 - `CanMsgs_MSG_NAME_unpack`: a function to unpack a given data array into a `CanMsgs_MSG_NAME_t` struct 
