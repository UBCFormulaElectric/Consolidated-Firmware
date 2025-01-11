/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdint.h>
#include <stdbool.h>
#ifndef THREAD_SAFE_CAN_PACKING
void io_canTx_set_lockfunction(
    void (*lock)(void),
    void (*unlock)(void)
);
#endif

/* ------------------------------- Structs -------------------------------- */

/**
 * Standard CAN message type.
 */
typedef struct
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t data[8];
} JsonCanMsg;

/* -------------------------------- Enums --------------------------------- */
typedef enum
{

    Bus_bus1 = 0,

    Bus_bus2 = 1,

} BusEnum;



typedef enum
{
    bus1Mode_default = 1 << 0,
    bus1Mode_debug = 1 << 1,
    
} bus1Mode;

typedef enum
{
    bus2Mode_default = 1 << 0,
    bus2Mode_debug = 1 << 1,
    
} bus2Mode;




/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialzie the IO CAN TX module.
 */
void io_canTx_init(
    void (*transmit_bus1_msg_func)(const JsonCanMsg*),void (*transmit_bus2_msg_func)(const JsonCanMsg*)
);

/**
 * Enable or disable a mode (only messages allowed for the enabled modes transmitted on the bus).
 */
void io_canTx_enableMode(CanMode mode, bool enable);

/**
 * Enqeue periodic CAN msgs of period 1000ms.
 */
void io_canTx_enqueue1HzMsgs(void);

/**
 * Enqeue periodic CAN msgs of period 10ms.
 */
void io_canTx_enqueue100HzMsgs(void);

/**
 * Enqueue periodic CAN messages whose cycle time does is not suitable for other periodic sending functions.
 */
void io_canTx_enqueueOtherPeriodicMsgs(uint32_t time_ms);


// send aperiodic messages


void io_canTx_VC_INVL_ReadWriteParamCommand_sendAperiodic();

void io_canTx_VC_INVR_ReadWriteParamCommand_sendAperiodic();

























