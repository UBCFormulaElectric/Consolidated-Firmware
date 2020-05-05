#include "Test_FlowMeter.h"

extern "C"
{
#include "App_FlowMeter.h"
}

FAKE_VALUE_FUNC(float, get_flow_rate);

void FlowMeterTest::SetUp()
{
    flow_meter = App_FlowMeter_Create(get_flow_rate);
    RESET_FAKE(get_flow_rate);
}

void FlowMeterTest::TearDown()
{
    ASSERT_TRUE(flow_meter != NULL);
    App_FlowMeter_Destroy(flow_meter);
    flow_meter = NULL;
}

void FlowMeterTest::TearDownFlowMeter(struct FlowMeter *&flow_meter_to_teardown)
{
    ASSERT_TRUE(flow_meter_to_teardown != NULL);
    App_FlowMeter_Destroy(flow_meter_to_teardown);
    flow_meter_to_teardown = NULL;
}

TEST_F(FlowMeterTest, check_if_flow_rate_is_updated_after_tick)
{
    get_flow_rate_fake.return_val = 1.0f;
    ASSERT_EQ(
        get_flow_rate_fake.return_val, App_FlowMeter_GetFlowRate(flow_meter));
}
