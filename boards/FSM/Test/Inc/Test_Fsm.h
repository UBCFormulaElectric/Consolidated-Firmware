#pragma once

#include <fff.h>
#include <gtest/gtest.h>

extern "C"
{
#include "App_FlowMeter.h"
}

class FsmTest : public testing::Test
{
  protected:
    struct FlowMeter *primary_flow_meter;
    struct FlowMeter *secondary_flow_meter;
};
