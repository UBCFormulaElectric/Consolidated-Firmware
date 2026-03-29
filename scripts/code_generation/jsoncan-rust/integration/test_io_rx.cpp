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
        app::can_utils::CAN_MSG_ECU_4_WARNINGS_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_4_WARNINGS_ID,
        app::can_utils::CAN_MSG_ECU_3_INFO_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_4_FAULTS_ID,
        app::can_utils::CAN_MSG_ECU_4_INFO_ID,
        app::can_utils::CAN_MSG_ECU_3_WARNINGS_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_3_FAULTS_ID,
        app::can_utils::CAN_MSG_ECU_4_FAULTS_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_3_WARNINGS_ID,
        app::can_utils::CAN_MSG_ECU_3_INFO_ID,
        app::can_utils::CAN_MSG_ECU_4_INFO_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_3_FAULTS_COUNTS_ID,
    };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can1(i), can1_present.contains(i));
    }

    const set can2_present = {
        app::can_utils::CAN_MSG_ECU_5_FAULTS_ID,      app::can_utils::CAN_MSG_ECU_5_FAULTS_COUNTS_ID,
        app::can_utils::CAN_MSG_ECU_5_INFO_COUNTS_ID, app::can_utils::CAN_MSG_ECU_5_INFO_ID,
        app::can_utils::CAN_MSG_ECU_5_WARNINGS_ID,    app::can_utils::CAN_MSG_ECU_5_WARNINGS_COUNTS_ID,
    };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can2(i), can2_present.contains(i));
    }

    const set can3_present = { app::can_utils::CAN_MSG_ECU_2_BASIC_SIGNAL_TYPES_ID };
    for (uint32_t i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io::can_rx::filterMessageId_can3(i), can3_present.contains(i));
    }
}