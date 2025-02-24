#include <gtest/gtest.h>

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class JsonCanAlertsTest : public testing::Test
{
  protected:
    void SetUp() { app_canTx_init(); }
};

TEST_F(JsonCanAlertsTest, test_false_on_init)
{
    // Fault false on init.
    ASSERT_EQ(app_canAlerts_ECU1_Fault_Fault1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), 0U);

    // Warning false on init.
    ASSERT_EQ(app_canAlerts_ECU1_Warning_Warning1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), 0U);
}

TEST_F(JsonCanAlertsTest, test_set_clear_once_fault)
{
    app_canAlerts_ECU1_Fault_Fault1_set(true);
    ASSERT_EQ(app_canAlerts_ECU1_Fault_Fault1_get(), true);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1_get(), true);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), 1);

    app_canAlerts_ECU1_Fault_Fault1_set(false);
    ASSERT_EQ(app_canAlerts_ECU1_Fault_Fault1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), 1);
}

TEST_F(JsonCanAlertsTest, test_set_clear_once_warning)
{
    app_canAlerts_ECU1_Warning_Warning1_set(true);
    ASSERT_EQ(app_canAlerts_ECU1_Warning_Warning1_get(), true);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1_get(), true);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), 1);

    app_canAlerts_ECU1_Warning_Warning1_set(false);
    ASSERT_EQ(app_canAlerts_ECU1_Warning_Warning1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1_get(), false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), 1);
}

TEST_F(JsonCanAlertsTest, test_set_clear_multiple_fault)
{
    for (int count = 1; count <= 7; count++)
    {
        app_canAlerts_ECU1_Fault_Fault1_set(true);
        app_canAlerts_ECU1_Fault_Fault1_set(false);
        ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), count);
    }

    // Can only count to 7 since we only give each alert 3 bits.
    app_canAlerts_ECU1_Fault_Fault1_set(true);
    app_canAlerts_ECU1_Fault_Fault1_set(false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), 7);

    app_canAlerts_ECU1_Fault_Fault1_set(true);
    app_canAlerts_ECU1_Fault_Fault1_set(false);
    ASSERT_EQ(app_canTx_ECU1_Fault_Fault1Count_get(), 7);
}

TEST_F(JsonCanAlertsTest, test_set_clear_multiple_warning)
{
    for (int count = 1; count <= 7; count++)
    {
        app_canAlerts_ECU1_Warning_Warning1_set(true);
        app_canAlerts_ECU1_Warning_Warning1_set(false);
        ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), count);
    }

    // Can only count to 7 since we only give each alert 3 bits.
    app_canAlerts_ECU1_Warning_Warning1_set(true);
    app_canAlerts_ECU1_Warning_Warning1_set(false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), 7);

    app_canAlerts_ECU1_Warning_Warning1_set(true);
    app_canAlerts_ECU1_Warning_Warning1_set(false);
    ASSERT_EQ(app_canTx_ECU1_Warning_Warning1Count_get(), 7);
}
