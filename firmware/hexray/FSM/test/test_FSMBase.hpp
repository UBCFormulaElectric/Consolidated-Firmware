#pragma once

#include "ecuTestBase.hpp"

class FSMBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        // Setup code for FSM tests can be added here
        register_task(jobs_run100Hz_tick, 10);
    }
    void board_teardown() override
    {
        // Teardown code for FSM tests can be added here
    }
    void tick_100hz() override
    {
        // Code to run every 10ms for FSM tests can be added here
    }
    void tick_1hz() override
    {
        // Code to run every 1000ms for FSM tests can be added here
    }

    public:
};