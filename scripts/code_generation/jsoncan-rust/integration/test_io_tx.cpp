#include <gtest/gtest.h>
#include <vector>
#include <array>
#include "io_canTx.hpp"

using namespace std;

class io_fixture : public testing::Test
{
  protected:
    void SetUp() override
    {
        io::can_tx::init([](const JsonCanMsg &) {}, [](const JsonCanMsg &) {}, [](const JsonCanMsg &) {});
        io::can_tx::setLockfunction([] -> void {}, [] -> void {});

        io::can_tx::enableMode_can1(app::can_utils::can1Mode::CAN1_MODE_default, true);
        io::can_tx::enableMode_can2(app::can_utils::can2Mode::CAN2_MODE_default, true);
        io::can_tx::enableMode_can3(app::can_utils::can3Mode::CAN3_MODE_default, true);
    }
};

TEST_F(io_fixture, test_lock_function)
{
    static bool locked = false, unlocked = false;
    io::can_tx::setLockfunction([] { locked = true; }, [] { unlocked = true; });

    io::can_tx::enqueue1HzMsgs();

    ASSERT_EQ(locked, true) << "Lock function not called";
    ASSERT_EQ(unlocked, true) << "Unlock function not called";
}

TEST_F(io_fixture, transmit_1hz_works)
{
    static vector<JsonCanMsg> tx_msg_1, tx_msg_2, tx_msg_3;
    io::can_tx::init(
        [](const JsonCanMsg &msg) { tx_msg_1.push_back(msg); }, [](const JsonCanMsg &msg) { tx_msg_2.push_back(msg); },
        [](const JsonCanMsg &msg) { tx_msg_3.push_back(msg); });

    io::can_tx::enqueue1HzMsgs();
    {
        constexpr array<uint32_t, 7> expected{ { 106, 107, 108, 109, 111, 100, 103 } };
        ASSERT_EQ(tx_msg_1.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_1[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array<uint32_t, 2> expected{ { 100, 103 } };
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
    io::can_tx::init(
        [](const JsonCanMsg &msg) { tx_msg_1.push_back(msg); }, [](const JsonCanMsg &msg) { tx_msg_2.push_back(msg); },
        [](const JsonCanMsg &msg) { tx_msg_3.push_back(msg); });

    io::can_tx::enqueue100HzMsgs();
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
    io::can_tx::init(
        [](const JsonCanMsg &msg) { tx_msg_1.push_back(msg); }, [](const JsonCanMsg &msg) { tx_msg_2.push_back(msg); },
        [](const JsonCanMsg &msg) { tx_msg_3.push_back(msg); });
    io::can_tx::enableMode_can1(app::can_utils::can1Mode::CAN1_MODE_debug, true);

    io::can_tx::enqueue1HzMsgs();
    {
        constexpr array<uint32_t, 8> expected{ { 106, 107, 108, 109, 110, 111, 100, 103 } };
        ASSERT_EQ(tx_msg_1.size(), expected.size());
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(tx_msg_1[i].std_id, expected[i]) << "Expected expected[" << i << "] to be correct";
        }
    }
    {
        constexpr array<uint32_t, 2> expected{ { 100, 103 } };
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