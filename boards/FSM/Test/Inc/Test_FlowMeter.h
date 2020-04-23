#pragma once

#include "Test_Fsm.h"

extern "C"
{
#include "App_FlowMeter.h"
}

DECLARE_FAKE_VALUE_FUNC(float, get_flow_rate);

