#include <gtest/gtest.h>
#include <set>
using namespace std;

extern "C"
{
#include "io_canRx.h"
#include "app_canUtils.h"
}

class io_rx_fixture : public testing::Test
{
};

TEST_F(io_rx_fixture, io_rx_filtering)
{
    constexpr int MAX_CANID    = 1 << 11;
    const set     can1_present = {
        CAN_MSG_ECU4_WARNINGS_COUNTS_ID,
        CAN_MSG_ECU4_WARNINGS_ID,
        CAN_MSG_ECU3_INFO_COUNTS_ID,
        CAN_MSG_ECU4_FAULTS_ID,
        CAN_MSG_ECU4_INFO_ID,
        CAN_MSG_ECU3_WARNINGS_COUNTS_ID,
        CAN_MSG_ECU3_FAULTS_ID,
        CAN_MSG_ECU4_FAULTS_COUNTS_ID,
        CAN_MSG_ECU3_WARNINGS_ID,
        CAN_MSG_ECU3_INFO_ID,
        CAN_MSG_ECU4_INFO_COUNTS_ID,
        CAN_MSG_ECU3_FAULTS_COUNTS_ID,
    };
    for (int i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io_canRx_filterMessageId_can1(i), can1_present.contains(i));
    }

    const set can2_present = {
        CAN_MSG_ECU5_FAULTS_ID, CAN_MSG_ECU5_FAULTS_COUNTS_ID, CAN_MSG_ECU5_INFO_COUNTS_ID,
        CAN_MSG_ECU5_INFO_ID,   CAN_MSG_ECU5_WARNINGS_ID,      CAN_MSG_ECU5_WARNINGS_COUNTS_ID,
    };
    for (int i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io_canRx_filterMessageId_can2(i), can2_present.contains(i));
    }

    const set can3_present = { CAN_MSG_ECU2_BASIC_SIGNAL_TYPES_ID };
    for (int i = 0; i <= MAX_CANID; i++)
    {
        EXPECT_EQ(io_canRx_filterMessageId_can3(i), can3_present.contains(i));
    }
}