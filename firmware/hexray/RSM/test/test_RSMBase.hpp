#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"

class RSMBaseTest: public EcuTestBase
{
    void board_setup() override
    {
        register_task(jobs_run100Hz_tick, 10);
    }

    void board_teardown() override {}
};