#pragma once
#include "ecuTestBase.hpp"
#include "jobs.hpp"

class CRITBaseTest : public EcuTestBase
{
    void board_setup() override { register_task(jobs_run10Hz_tick, 1000); };
    void board_teardown() override{};
};
