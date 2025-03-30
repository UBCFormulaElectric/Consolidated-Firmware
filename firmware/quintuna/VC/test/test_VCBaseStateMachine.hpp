#pragma once
#include "ecuTestBase.hpp"

class VcBaseStateMachineTest : public EcuTestBase
{
    void tick_100hz() override;
    void tick_1hz() override;
    void board_setup() override;
    void board_teardown() override;
};