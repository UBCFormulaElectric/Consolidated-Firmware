#include <gtest/gtest.h>
#include <vector>

using namespace std;

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_canTx.h"
}

class JsonCanLoopbackTest : public testing::Test
{
  public:
    static vector<JsonCanMsg> tx_msg_1, tx_msg_2, tx_msg_3;

    static void push_tx_1Hz_msg(const JsonCanMsg *msg) { tx_msg_1.push_back(*msg); }
    static void push_tx_10Hz_msg(const JsonCanMsg *msg) { tx_msg_2.push_back(*msg); }
    static void push_tx_100Hz_msg(const JsonCanMsg *msg) { tx_msg_3.push_back(*msg); }

  protected:
    void SetUp() override
    {
        tx_msg_1.clear();
        tx_msg_2.clear();
        tx_msg_3.clear();
        io_canTx_init(
            [](const JsonCanMsg *msg) { tx_msg_1.push_back(*msg); }, [](const JsonCanMsg *msg)
            { tx_msg_2.push_back(*msg); }, [](const JsonCanMsg *msg) { tx_msg_3.push_back(*msg); });

        io_canTx_enableMode_can1(CAN1_MODE_default, true);
        io_canTx_enableMode_can2(CAN2_MODE_default, true);
        io_canTx_enableMode_can3(CAN3_MODE_default, true);

        app_canTx_init();
        app_canRx_init();
    }
};

vector<JsonCanMsg> JsonCanLoopbackTest::tx_msg_1;
vector<JsonCanMsg> JsonCanLoopbackTest::tx_msg_2;
vector<JsonCanMsg> JsonCanLoopbackTest::tx_msg_3;

TEST_F(JsonCanLoopbackTest, transmit_1hz_works)
{
    io_canTx_enqueue1HzMsgs();
    {
        constexpr array expected = { 106, 107, 108, 109, 100, 103 };
        ASSERT_EQ(tx_msg_1.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_1[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array expected = { 100, 103 };
        ASSERT_EQ(tx_msg_2.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_2[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array<int, 0> expected = {};
        ASSERT_EQ(tx_msg_3.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_3[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
}

TEST_F(JsonCanLoopbackTest, transmit_100hz_works)
{
    io_canTx_enqueue100HzMsgs();
    {
        constexpr array<int, 0> expected = {};
        ASSERT_EQ(tx_msg_1.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_1[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array<int, 0> expected = {};
        ASSERT_EQ(tx_msg_2.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_2[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array<int, 0> expected = {};
        ASSERT_EQ(tx_msg_3.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_3[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
}
