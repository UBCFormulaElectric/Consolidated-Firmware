#include <gtest/gtest.h>
#include <set>
using namespace std;

#include "io_canRx.hpp"
#include "app_canUtils.hpp"

class io_rx_fixture : public testing::Test
{
};

TEST_F(io_rx_fixture, io_rx_filtering)
{
    constexpr int MAX_CANID    = 1 << 11;
    const set     can1_present = {
        app::can_utils::ECU4_WarningsCounts_Signals::MSG_ID, app::can_utils::ECU4_Warnings_Signals::MSG_ID,
        app::can_utils::ECU3_InfoCounts_Signals::MSG_ID,     app::can_utils::ECU4_Faults_Signals::MSG_ID,
        app::can_utils::ECU4_Info_Signals::MSG_ID,           app::can_utils::ECU3_WarningsCounts_Signals::MSG_ID,
        app::can_utils::ECU3_Faults_Signals::MSG_ID,         app::can_utils::ECU4_FaultsCounts_Signals::MSG_ID,
        app::can_utils::ECU3_Warnings_Signals::MSG_ID,       app::can_utils::ECU3_Info_Signals::MSG_ID,
        app::can_utils::ECU4_InfoCounts_Signals::MSG_ID,     app::can_utils::ECU3_FaultsCounts_Signals::MSG_ID,
    };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can1(i), can1_present.contains(i));
    }

    const set can2_present = {
        app::can_utils::ECU5_Faults_Signals::MSG_ID,     app::can_utils::ECU5_FaultsCounts_Signals::MSG_ID,
        app::can_utils::ECU5_InfoCounts_Signals::MSG_ID, app::can_utils::ECU5_Info_Signals::MSG_ID,
        app::can_utils::ECU5_Warnings_Signals::MSG_ID,   app::can_utils::ECU5_WarningsCounts_Signals::MSG_ID,
    };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can2(i), can2_present.contains(i));
    }

    const set can3_present = { app::can_utils::ECU2_BasicSignalTypes_Signals::MSG_ID };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can3(i), can3_present.contains(i));
    }
}