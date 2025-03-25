#include "test_VCBaseStateMachine.hpp"

extern "C"
{
#include "jobs.h"
}

void VcBaseStateMachineTest::board_setup()
{
    jobs_init();
}

void VcBaseStateMachineTest::board_teardown() {}

void VcBaseStateMachineTest::tick_100hz()
{
    jobs_run100Hz_tick();
}

void VcBaseStateMachineTest::tick_1hz()
{
    jobs_run1Hz_tick();
}