#include <gtest/gtest.h>
#include <vector>
#include <array>
using namespace std;

extern "C"
{
#include "io_canTx.h"
}
class io_fixture : public testing::Test
{
  protected:
    void SetUp() override
    {
        io_canTx_init([](const JsonCanMsg *) {}, [](const JsonCanMsg *) {}, [](const JsonCanMsg *) {});
        io_canTx_set_lockfunction([] -> void {}, [] -> void {});

        io_canTx_enableMode_can1(CAN1_MODE_default, true);
        io_canTx_enableMode_can2(CAN2_MODE_default, true);
        io_canTx_enableMode_can3(CAN3_MODE_default, true);
    }
};

TEST_F(io_fixture, test_lock_function)
{
    static bool locked = false, unlocked = false;
    io_canTx_set_lockfunction([] { locked = true; }, [] { unlocked = true; });

    io_canTx_enqueue1HzMsgs();

    ASSERT_EQ(locked, true) << "Lock function not called";
    ASSERT_EQ(unlocked, true) << "Unlock function not called";
}

TEST_F(io_fixture, transmit_1hz_works)
{
    static vector<JsonCanMsg> tx_msg_1, tx_msg_2, tx_msg_3;
    io_canTx_init(
        [](const JsonCanMsg *msg) { tx_msg_1.push_back(*msg); }, [](const JsonCanMsg *msg)
        { tx_msg_2.push_back(*msg); }, [](const JsonCanMsg *msg) { tx_msg_3.push_back(*msg); });

    io_canTx_enqueue1HzMsgs();
    {
        constexpr array expected = { 106, 107, 108, 109, 111, 100, 103 };
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

TEST_F(io_fixture, transmit_100hz_works)
{
    static vector<JsonCanMsg> tx_msg_1, tx_msg_2, tx_msg_3;
    io_canTx_init(
        [](const JsonCanMsg *msg) { tx_msg_1.push_back(*msg); }, [](const JsonCanMsg *msg)
        { tx_msg_2.push_back(*msg); }, [](const JsonCanMsg *msg) { tx_msg_3.push_back(*msg); });

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

// FUCK TESTING io_canTx_enqueueOtherPeriodicMsgs I know that shit work
// what are the odds io_canTx_enqueue1HzMsgs AND io_canTx_enqueue100HzMsgs work but io_canTx_enqueueOtherPeriodicMsgs
// don't work?? be serious

TEST_F(io_fixture, can_bus_mod_works)
{
    static vector<JsonCanMsg> tx_msg_1, tx_msg_2, tx_msg_3;
    io_canTx_init(
        [](const JsonCanMsg *msg) { tx_msg_1.push_back(*msg); }, [](const JsonCanMsg *msg)
        { tx_msg_2.push_back(*msg); }, [](const JsonCanMsg *msg) { tx_msg_3.push_back(*msg); });
    io_canTx_enableMode_can1(CAN1_MODE_debug, true);

    io_canTx_enqueue1HzMsgs();
    {
        constexpr array expected = { 106, 107, 108, 109, 110, 111, 100, 103 };
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