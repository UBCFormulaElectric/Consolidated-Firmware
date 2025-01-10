/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canTx.h"
#include <string.h>
#include "app_canTx.h"
#include "app_canUtils.h"

#ifndef THREAD_SAFE_CAN_PACKING
#include <FreeRTOS.h>
#include <portmacro.h>
#endif

/* --------------------------- Static Variables --------------------------- */

static uint32_t can_mode_;
static void (*transmit_func_)(const JsonCanMsg* tx_msg);

static uint32_t can_mode_;
static void (*transmit_func_)(const JsonCanMsg* tx_msg);


/* --------------------- Static Function Definitions ---------------------- */




/* --------------------- Public Function Definitions ---------------------- */

void io_canTx_init( 
    void (*transmit__msg_func)(const JsonCanMsg*),
    void (*transmit__msg_func)(const JsonCanMsg*)
    
)
{
    transmit_func_ = transmit__msg_func;
    transmit_func_ = transmit__msg_func;
    
}


void io_canTx_enableMode_(CanMode_ mode, bool enable)
{
    if (enable)
    {
        can_mode_ |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_ &= ~((uint32_t)mode); // Disable mode
    }
}

void io_canTx_enableMode_(CanMode_ mode, bool enable)
{
    if (enable)
    {
        can_mode_ |= (uint32_t)mode; // Enable mode
        
    }
    else
    {
        can_mode_ &= ~((uint32_t)mode); // Disable mode
    }
}




void io_canTx_enqueue1HzMsgs()
{
}

void io_canTx_enqueue100HzMsgs()
{
}

void io_canTx_enqueueOtherPeriodicMsgs(uint32_t time_ms)
{
    
}
