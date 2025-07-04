#include "app_canUtils.h"
#include <cstring>
#include <gtest/gtest.h>

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_canRx.h"
}

class JsonCanTablesTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        app_canTx_init();
        app_canRx_init();
    }
};

TEST_F(JsonCanTablesTest, test_tx_zero_init)
{
    const ECU1_BasicSignalTypes_Signals *data = app_canTx_ECU1_BasicSignalTypes_getData();
    ASSERT_EQ(data->ECU1_Boolean1_value, false);
    ASSERT_EQ(data->ECU1_Boolean2_value, false);
    ASSERT_EQ(data->ECU1_Enum_value, ENUM_EX_0);
    ASSERT_EQ(data->ECU1_UInt8_value, 0);
    ASSERT_EQ(data->ECU1_UInt16_value, 0);
    ASSERT_EQ(data->ECU1_UInt32_value, 0);
}

TEST_F(JsonCanTablesTest, test_tx_basic_signals)
{
    // Normal test.
    app_canTx_ECU1_Boolean1_set(true);
    app_canTx_ECU1_Boolean2_set(false);
    app_canTx_ECU1_Enum_set(ENUM_EX_1);
    app_canTx_ECU1_UInt8_set(54);
    app_canTx_ECU1_UInt32_set(0xABCDEF12);

    {
        const ECU1_BasicSignalTypes_Signals *data = app_canTx_ECU1_BasicSignalTypes_getData();
        ASSERT_EQ(data->ECU1_Boolean1_value, true);
        ASSERT_EQ(data->ECU1_Boolean2_value, false);
        ASSERT_EQ(data->ECU1_Enum_value, ENUM_EX_1);
        ASSERT_EQ(data->ECU1_UInt8_value, 54);
        ASSERT_EQ(data->ECU1_UInt16_value, 0); // Still zero
        ASSERT_EQ(data->ECU1_UInt32_value, 0xABCDEF12);
    }

    // Test clamping.
    // ReSharper disable once CppCompileTimeConstantCanBeReplacedWithBooleanConstant
    app_canTx_ECU1_Boolean1_set(0xFF); // NOLINT(*-use-bool-literals)

    // ReSharper disable once CppCompileTimeConstantCanBeReplacedWithBooleanConstant
    app_canTx_ECU1_Boolean2_set(0xFF); // NOLINT(*-use-bool-literals)

    app_canTx_ECU1_Enum_set(static_cast<EnumExample>(0xFF));
    app_canTx_ECU1_UInt8_set(0xFFFFFFFF);
    app_canTx_ECU1_UInt16_set(0xFFFFFFFF);
    app_canTx_ECU1_UInt32_set(0xFFFFFFFF);

    {
        const ECU1_BasicSignalTypes_Signals *data = app_canTx_ECU1_BasicSignalTypes_getData();
        ASSERT_EQ(data->ECU1_Boolean1_value, true);
        ASSERT_EQ(data->ECU1_Boolean2_value, true);
        ASSERT_EQ(data->ECU1_Enum_value, ENUM_EX_2);
        ASSERT_EQ(data->ECU1_UInt8_value, 0xFF);
        ASSERT_EQ(data->ECU1_UInt16_value, 0xFFFF);
        ASSERT_EQ(data->ECU1_UInt32_value, 0xFFFFFFFF);
    }
}

TEST_F(JsonCanTablesTest, test_tx_decimals)
{
    // Normal test.
    app_canTx_ECU1_Decimal1_set(-54.78);
    app_canTx_ECU1_Decimal2_set(2.891);

    {
        const ECU1_DecimalNumbers_Signals *data = app_canTx_ECU1_DecimalNumbers_getData();
        ASSERT_NEAR(data->ECU1_Decimal1_value, -54.78, 1e-5);
        ASSERT_NEAR(data->ECU1_Decimal2_value, 2.891, 1e-5);
    }

    // Test clamping.
    app_canTx_ECU1_Decimal1_set(-120);
    app_canTx_ECU1_Decimal2_set(3.5);

    {
        const ECU1_DecimalNumbers_Signals *data = app_canTx_ECU1_DecimalNumbers_getData();
        ASSERT_EQ(data->ECU1_Decimal1_value, -100);
        ASSERT_EQ(data->ECU1_Decimal2_value, 3);
    }
}

TEST_F(JsonCanTablesTest, test_rx_zero_init)
{
    ASSERT_EQ(app_canRx_ECU2_Boolean1_get(), false);
    ASSERT_EQ(app_canRx_ECU2_Boolean2_get(), false);
    ASSERT_EQ(app_canRx_ECU2_Enum_get(), ENUM_EX_0);
    ASSERT_EQ(app_canRx_ECU2_UInt8_get(), 0);
    ASSERT_EQ(app_canRx_ECU2_UInt16_get(), 0);
    ASSERT_EQ(app_canRx_ECU2_UInt32_get(), 0);
}

TEST_F(JsonCanTablesTest, test_rx_basic_signals)
{
    // Normal test.
    app_canRx_ECU2_Boolean1_update(true);
    app_canRx_ECU2_Boolean2_update(false);
    app_canRx_ECU2_Enum_update(ENUM_EX_1);
    app_canRx_ECU2_UInt8_update(54);
    app_canRx_ECU2_UInt32_update(0xABCDEF12);

    {
        ASSERT_EQ(app_canRx_ECU2_Boolean1_get(), true);
        ASSERT_EQ(app_canRx_ECU2_Boolean2_get(), false);
        ASSERT_EQ(app_canRx_ECU2_Enum_get(), ENUM_EX_1);
        ASSERT_EQ(app_canRx_ECU2_UInt8_get(), 54);
        ASSERT_EQ(app_canRx_ECU2_UInt16_get(), 0); // Still zero
        ASSERT_EQ(app_canRx_ECU2_UInt32_get(), 0xABCDEF12);
    }

    // Test clamping and unpacking.
    JsonCanMsg test_rx_msg = { .std_id = CAN_MSG_ECU2_BASIC_SIGNAL_TYPES_ID, .dlc = 8, .data = { 0 } };
    memset(&test_rx_msg.data, 0xFF, 8);
    io_canRx_updateRxTableWithMessage(&test_rx_msg);
    {
        ASSERT_EQ(app_canRx_ECU2_Boolean1_get(), true);
        ASSERT_EQ(app_canRx_ECU2_Boolean2_get(), true);
        ASSERT_EQ(app_canRx_ECU2_Enum_get(), ENUM_EX_2);
        ASSERT_EQ(app_canRx_ECU2_UInt8_get(), 0xFF);
        ASSERT_EQ(app_canRx_ECU2_UInt16_get(), 0xFFFF);
        ASSERT_EQ(app_canRx_ECU2_UInt32_get(), 0xFFFFFFFF);
    }
}
