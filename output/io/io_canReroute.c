/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canRx.h"




static void (*transmit_func_bus1)(const JsonCanMsg* tx_msg);

static void (*transmit_func_bus2)(const JsonCanMsg* tx_msg);


static void do_transmit(const JsonCanMsg* tx_msg)
{
    transmit_func_bus1(tx_msg);
    transmit_func_bus2(tx_msg);
    
}





void io_canReroute_init( 
    void (*transmit_bus1_msg_func)(const JsonCanMsg*),
    void (*transmit_bus2_msg_func)(const JsonCanMsg*)
    
)
{
    transmit_func_bus1 = transmit_bus1_msg_func;
    transmit_func_bus2 = transmit_bus2_msg_func;
    
}

void io_canReroute(const JsonCanMsg* rx_msg)
{
    switch(rx_msg->std_id)
    {
        case CANMSG_BMS_VITALS_ID:
        {
            // Reroute message
            do_transmit(rx_msg);
        }

        default:
            break;
    }
}
