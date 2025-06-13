#pragma once
#include "ecuTestBase.hpp"

extern "C"
{
}

class DAMBaseTest : public EcuTestBase
{
    void board_setup() override{};
    void board_teardown() override {}
    void tick_100hz() override {}
    void tick_1hz() override {}
};