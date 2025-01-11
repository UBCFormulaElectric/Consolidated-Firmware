/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canRx.h"



static void (*transmit_func_bus1)(const JsonCanMsg* tx_msg);
static void (*transmit_func_bus2)(const JsonCanMsg* tx_msg);






void io_canReroute_init( 
    void (*transmit__msg_func)(const JsonCanMsg*),
    void (*transmit__msg_func)(const JsonCanMsg*)
    
)
{
    transmit_func_ = transmit__msg_func;
    transmit_func_ = transmit__msg_func;
    
}

void io_canReroute(const JsonCanMsg* rx_msg)
{
    switch(rx_msg->std_id)
    {
        case CANMSG_BMS_VITALS_ID:
        {
            // Reroute message from BMS to FSM
            // via bus2
            transmit_func_bus2(rx_msg);
            
        }
        

        default:
            break;
    }
}
