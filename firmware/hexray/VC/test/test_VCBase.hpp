#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"

class VCBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        register_task(jobs_run1Hz_tick, 1000);
        register_task(jobs_run100Hz_tick, 10);
        register_task(jobs_run1kHz_tick, 1);

        jobs_init();
    }
    void board_teardown() override {}
};