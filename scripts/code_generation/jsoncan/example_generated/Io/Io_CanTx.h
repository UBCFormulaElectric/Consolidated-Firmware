/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------- Enums --------------------------------- */

typedef enum
{
    CAN_MODE_DEFAULT = 1 << 0,
    CAN_MODE_DEBUG = 1 << 1,
} CanMode;

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Enable or disable a mode (only messages allowed for the enabled modes transmitted on the bus).
 */
void Io_CanTx_EnableMode(CanMode mode, bool enable);

/**
 * Enqeue periodic CAN msgs of period 1000ms.
 */
void Io_CanTx_Enqueue1HzMsgs(void);

/**
 * Enqeue periodic CAN msgs of period 10ms.
 */
void Io_CanTx_Enqueue100HzMsgs(void);

/**
 * Enqueue periodic CAN messages whose cycle time does is not suitable for other periodic sending functions.
 */
void Io_CanTx_EnqueueOtherPeriodicMsgs(uint32_t time_ms);

/**
 * Pack and send the aperiodic TX msg JCT_WarningsTest.
 */
void Io_CanTx_JCT_WarningsTest_SendAperiodic(void);

