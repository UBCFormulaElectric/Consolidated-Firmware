#include <cstring>
#include <gtest/gtest.h>

#include "app_canUtils.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_canRx.hpp"

class JsonCanTablesTest : public testing::Test
{
  protected:
    void SetUp() override {}
};

TEST_F(JsonCanTablesTest, test_tx_zero_init)
{
    const app::can_utils::ECU1_BasicSignalTypes_Signals &data = app::can_tx::ECU1_BasicSignalTypes_getData();
    ASSERT_EQ(data.ECU1_Boolean1_value, false);
    ASSERT_EQ(data.ECU1_Boolean2_value, false);
    ASSERT_EQ(data.ECU1_Enum_value, app::can_utils::EnumExample::ENUM_EX_0);
    ASSERT_EQ(data.ECU1_UInt8_value, 0);
    ASSERT_EQ(data.ECU1_UInt16_value, 0);
    ASSERT_EQ(data.ECU1_UInt32_value, 0);
}

TEST_F(JsonCanTablesTest, test_tx_basic_signals)
{
    // Normal test.
    app::can_tx::ECU1_Boolean1_set(true);
    app::can_tx::ECU1_Boolean2_set(false);
    app::can_tx::ECU1_Enum_set(app::can_utils::EnumExample::ENUM_EX_1);
    app::can_tx::ECU1_UInt8_set(54);
    app::can_tx::ECU1_UInt32_set(0xABCDEF12);

    {
        const app::can_utils::ECU1_BasicSignalTypes_Signals &data = app::can_tx::ECU1_BasicSignalTypes_getData();
        ASSERT_EQ(data.ECU1_Boolean1_value, true);
        ASSERT_EQ(data.ECU1_Boolean2_value, false);
        ASSERT_EQ(data.ECU1_Enum_value, app::can_utils::EnumExample::ENUM_EX_1);
        ASSERT_EQ(data.ECU1_UInt8_value, 54);
        ASSERT_EQ(data.ECU1_UInt16_value, 0); // Still zero
        ASSERT_EQ(data.ECU1_UInt32_value, 0xABCDEF12);
    }

    // Test clamping.
    // ReSharper disable once CppCompileTimeConstantCanBeReplacedWithBooleanConstant
    app::can_tx::ECU1_Boolean1_set(0xFF); // NOLINT(*-use-bool-literals)

    // ReSharper disable once CppCompileTimeConstantCanBeReplacedWithBooleanConstant
    app::can_tx::ECU1_Boolean2_set(0xFF); // NOLINT(*-use-bool-literals)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    app::can_tx::ECU1_Enum_set(app::can_utils::EnumExample::ENUM_EX_2);
    app::can_tx::ECU1_UInt8_set(0xFFFFFFFF);
    app::can_tx::ECU1_UInt16_set(0xFFFFFFFF);
    app::can_tx::ECU1_UInt32_set(0xFFFFFFFF);
#pragma GCC diagnostic pop

    {
        const app::can_utils::ECU1_BasicSignalTypes_Signals &data = app::can_tx::ECU1_BasicSignalTypes_getData();
        ASSERT_EQ(data.ECU1_Boolean1_value, true);
        ASSERT_EQ(data.ECU1_Boolean2_value, true);
        ASSERT_EQ(data.ECU1_Enum_value, app::can_utils::EnumExample::ENUM_EX_2);
        ASSERT_EQ(data.ECU1_UInt8_value, 0xFF);
        ASSERT_EQ(data.ECU1_UInt16_value, 0xFFFF);
        ASSERT_EQ(data.ECU1_UInt32_value, 0xFFFFFFFF);
    }
}

TEST_F(JsonCanTablesTest, test_tx_decimals)
{
    // Normal test.
    app::can_tx::ECU1_Decimal1_set(-54.78f);
    app::can_tx::ECU1_Decimal2_set(2.891f);

    {
        const app::can_utils::ECU1_DecimalNumbers_Signals &data = app::can_tx::ECU1_DecimalNumbers_getData();
        ASSERT_NEAR(data.ECU1_Decimal1_value, -54.78, 1e-5);
        ASSERT_NEAR(data.ECU1_Decimal2_value, 2.891, 1e-5);
    }

    // Test clamping.
    app::can_tx::ECU1_Decimal1_set(-120);
    app::can_tx::ECU1_Decimal2_set(3.5);

    {
        const app::can_utils::ECU1_DecimalNumbers_Signals &data = app::can_tx::ECU1_DecimalNumbers_getData();
        ASSERT_EQ(data.ECU1_Decimal1_value, -100);
        ASSERT_EQ(data.ECU1_Decimal2_value, 3);
    }
}

TEST_F(JsonCanTablesTest, test_rx_zero_init)
{
    ASSERT_EQ(app::can_rx::ECU2_Boolean1_get(), false);
    ASSERT_EQ(app::can_rx::ECU2_Boolean2_get(), false);
    ASSERT_EQ(app::can_rx::ECU2_Enum_get(), app::can_utils::EnumExample::ENUM_EX_0);
    ASSERT_EQ(app::can_rx::ECU2_UInt8_get(), 0);
    ASSERT_EQ(app::can_rx::ECU2_UInt16_get(), 0);
    ASSERT_EQ(app::can_rx::ECU2_UInt32_get(), 0);
}

TEST_F(JsonCanTablesTest, test_rx_basic_signals)
{
    // Normal test.
    app::can_rx::ECU2_Boolean1_update(true);
    app::can_rx::ECU2_Boolean2_update(false);
    app::can_rx::ECU2_Enum_update(app::can_utils::EnumExample::ENUM_EX_1);
    app::can_rx::ECU2_UInt8_update(54);
    app::can_rx::ECU2_UInt32_update(0xABCDEF12);

    {
        ASSERT_EQ(app::can_rx::ECU2_Boolean1_get(), true);
        ASSERT_EQ(app::can_rx::ECU2_Boolean2_get(), false);
        ASSERT_EQ(app::can_rx::ECU2_Enum_get(), app::can_utils::EnumExample::ENUM_EX_1);
        ASSERT_EQ(app::can_rx::ECU2_UInt8_get(), 54);
        ASSERT_EQ(app::can_rx::ECU2_UInt16_get(), 0); // Still zero
        ASSERT_EQ(app::can_rx::ECU2_UInt32_get(), 0xABCDEF12);
    }

    // Test clamping and unpacking.
    JsonCanMsg test_rx_msg = { app::can_utils::CAN_MSG_ECU_2_BASIC_SIGNAL_TYPES_ID, 8, {} };
    memset(&test_rx_msg.data, 0xFF, 8);
    io::can_rx::updateRxTableWithMessage(test_rx_msg);
    {
        ASSERT_EQ(app::can_rx::ECU2_Boolean1_get(), true);
        ASSERT_EQ(app::can_rx::ECU2_Boolean2_get(), true);
        ASSERT_EQ(app::can_rx::ECU2_Enum_get(), static_cast<app::can_utils::EnumExample>(0x03));
        ASSERT_EQ(app::can_rx::ECU2_UInt8_get(), 0xFF);
        ASSERT_EQ(app::can_rx::ECU2_UInt16_get(), 0xFFFF);
        ASSERT_EQ(app::can_rx::ECU2_UInt32_get(), 0xFFFFFFFF);
    }
}
