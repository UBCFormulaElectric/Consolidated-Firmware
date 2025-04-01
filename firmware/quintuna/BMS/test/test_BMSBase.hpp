#pragma once
#include "ecuTestBase.hpp"

extern "C"
{
#include "jobs.h"
}

class BMSBaseTest : public EcuTestBase
{
    void board_setup() override { jobs_init(); }
    void board_teardown() override {}
    void tick_100hz() override { jobs_run100Hz_tick(); }
    void tick_1hz() override { jobs_run1Hz_tick(); }
};