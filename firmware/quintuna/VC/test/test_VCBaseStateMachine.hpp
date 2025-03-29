#pragma once
#include "test_baseStateMachineTest.h"

class VcBaseStateMachineTest : public BaseStateMachineTest
{
    void tick_100hz() override;
    void tick_1hz() override;
    void board_setup() override;
    void board_teardown() override;
};