#include "app_canUtils.h"
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
    void SetUp()
    {
        io_canTx_init(
            [](const JsonCanMsg *msg)
            {
                JsonCanMsg rx_msg;
                memcpy(&rx_msg, msg, sizeof(JsonCanMsg));

                switch (msg->std_id)
                {
                    case CAN_MSG_ECU1_BASIC_SIGNAL_TYPES_ID:
                        rx_msg.std_id = CAN_MSG_ECU2_BASIC_SIGNAL_TYPES_ID;
                        break;
                    default:
                        return;
                }

                io_canRx_updateRxTableWithMessage(&rx_msg);
            });

        io_canTx_enableMode_bus1(BUS1_MODE_default, true);

        app_canTx_init();
        app_canRx_init();
    }
};

TEST_F(JsonCanLoopbackTest, test_loopback)
{
    app_canTx_ECU1_Boolean1_set(true);
    app_canTx_ECU1_Boolean2_set(false);
    app_canTx_ECU1_Enum_set(ENUM_EX_2);
    app_canTx_ECU1_UInt8_set(78);
    app_canTx_ECU1_UInt16_set(12);
    app_canTx_ECU1_UInt32_set(0xFAFAFA);

    io_canTx_enqueue1HzMsgs();

    ASSERT_EQ(app_canRx_ECU2_Boolean1_get(), true);
    ASSERT_EQ(app_canRx_ECU2_Boolean2_get(), false);
    ASSERT_EQ(app_canRx_ECU2_Enum_get(), ENUM_EX_2);
    ASSERT_EQ(app_canRx_ECU2_UInt8_get(), 78);
    ASSERT_EQ(app_canRx_ECU2_UInt16_get(), 12);
    ASSERT_EQ(app_canRx_ECU2_UInt32_get(), 0xFAFAFA);
}
