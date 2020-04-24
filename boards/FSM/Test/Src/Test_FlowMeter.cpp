#include "Test_Fsm.h"

FAKE_VALUE_FUNC(float, get_flow_rate);

class FlowMeterTest : public testing::Test
{
  protected:
    struct FlowMeter *flow_meter;

    void SetUp() override
    {
        flow_meter = App_FlowMeter_Create(get_flow_rate);
        RESET_FAKE(get_flow_rate);
    }

    void TearDown() override
    {
        ASSERT_TRUE(flow_meter != NULL);
        App_FlowMeter_Destroy(flow_meter);
        flow_meter = NULL;
    }
};

TEST_F(FlowMeterTest, check_if_flow_rate_is_updated_after_tick)
{
    get_flow_rate_fake.return_val          = 1.0f;
    App_FlowMeter_Tick(flow_meter);
    ASSERT_EQ(
        get_flow_rate_fake.return_val, App_FlowMeter_GetFlowRate(flow_meter));
}
