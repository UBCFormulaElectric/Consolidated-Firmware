#include <cstring>
#include <gtest/gtest.h>

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "io_canTx.h"
#include "io_canRx.h"
}

class JsonCanLoopbackTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        io_canTx_init(
            [](const JsonCanMsg *msg)
            {
                // JsonCanMsg rx_msg;
                // memcpy(&rx_msg, msg, sizeof(JsonCanMsg));
                //
                // switch (msg->std_id)
                // {
                //     case CAN_MSG_ECU1_BASIC_SIGNAL_TYPES_ID:
                //         rx_msg.std_id = CAN_MSG_ECU2_BASIC_SIGNAL_TYPES_ID;
                //         break;
                //     default:
                //         return;
                // }
                //
                // io_canRx_updateRxTableWithMessage(&rx_msg);
            },
            [](const JsonCanMsg *msg)
            {
                // pass
            },
            [](const JsonCanMsg *msg)
            {
                // pass
            });

        // io_canTx_enableMode_bus1(BUS1_MODE_default, true);
        io_canTx_enableMode_can1(CAN1_MODE_default, true);
        io_canTx_enableMode_can2(CAN2_MODE_default, true);
        io_canTx_enableMode_can3(CAN3_MODE_default, true);

        app_canTx_init();
        app_canRx_init();
    }
};