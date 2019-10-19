## Overview
This repository contains our CAN library for STM32 F3 microcontrollers. The goal is to abstract away low-level details, provide a set of easy-to-use CAN helper functions, and enforce consistency for CAN communication across every PCB on the vehicle.

The bxCAN controller has 3 hardware transmit mailboxes, which means it can only hold 3 Tx messages at any given time. If the user attemps to transmit a message while all three transmit mailboxes are occupied, we store this message in a **software** FIFO queue. Messages in this FIFO queue will be automatically de-queued and transmitted when any of the transmit mailboxes becomes available. This FIFO queue has a fixed size of 20 levels deep, which is more-or-less arbitrary but it should be sufficient in most cases. If the FIFO queue were to overflow, a CAN message will be transmitted. If we ever see this CAN message in the data logger, we can increase the FIFO queue size accordingly.

## CAN Filters
The CAN receive filters activated are dependent on what is defined in each board-specific `Io_Can.c`

For example,
```
// ./boards/PDM/Src/Io_Can.c
static CanMaskFilterConfig_Struct mask_filters[2] =
{
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
```

This tells us for `PDM`, we have activated two CAN filters - the **DCM filter** and the **Shared filter**. We can then check `SharedCan.h` to see which CAN IDs each of these two filters will accept:

## Making Changes to CAN Messages
0. Edit the `.dbc` using `PCAN-View` (which is free to download)
0. Run `generate_c_code_from_sym.py` to generate `CanMsgs.c` and `CanMsgs.h` based on the `.dbc`.

## Working With The Generated C Code
For every CAN message (hereafter generally referred to as `MSG_NAME`), there will be:
- `CanMsgs_MSG_NAME_t`: a struct that is setup to hold the various elements of the message
- `CanMsgs_MSG_NAME_pack`: a function to pack the `CanMsgs_MSG_NAME_t` struct into a data array that we can transmit over CAN  
- `CanMsgs_MSG_NAME_unpack`: a function to unpack a given data array into a `CanMsgs_MSG_NAME_t` struct 

There are other functions in `CanMsgs.h`, but in almost all cases these should **not** be used.

## Reserved Ranges
*These are ranges that have already been reserved for specific purposes/boards. They should _not_ be used for **anything** other then their intended purpose.
- `0xffffffff to 0x1F` (**BMS**): CAN messages sent from the BMS
- `0x1f to 0x3F` (**DCM**): CAN messages sent from the DCM
- `0x3f to 0x5F` (**FSM**): CAN messages sent from the FSM
- `0x5f to 0x7F` (**PDM**): CAN messages sent from the FSM
- `0x7f to 0x9F` (**Shared**): general CAN messages that could be sent from anywhere
- `0x18f to 0x19F` (**BAMOCAR Tx**): CAN messages sent from our BAMOCAR inverter 
- `0x20f to 0x21F` (**BAMOCAR Rx**): CAN messages received by our BAMOCAR inverter
