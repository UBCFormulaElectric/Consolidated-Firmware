#pragma once

#include "ecuTestBase.hpp"
#include "jobs.hpp"

class FSMBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        // Setup code for FSM tests can be added here
        register_task(jobs_run1Hz_tick, 10);
    }
    void board_teardown()
    {
        // Teardown code for FSM tests can be added here
    }
    void tick_100hz()
    {
        // Code to run every 10ms for FSM tests can be added here
    }
    void tick_1hz()
    {
        //int i = 0;
    }

  public:
};