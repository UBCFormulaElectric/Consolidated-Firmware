#pragma once

#include "Test_Fsm.h"

class FlowMeterTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;
    void TearDownFlowMeter(struct FlowMeter *&flow_meter);

    struct FlowMeter *flow_meter;
};
