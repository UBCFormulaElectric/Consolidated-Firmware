#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"

class RSMBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        jobs_init();
        register_task(jobs_run1Hz_tick, 1000);
        register_task(jobs_run100Hz_tick, 100);
        register_task(jobs_run1kHz_tick, 1);
    }

    void board_teardown() override {}
};