#include <stdio.h>
#include <stdlib.h>

// Fake io Layer.
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"
#include "fake_io_loadCell.hpp"

// App layer.
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_steering.h"
#include "app_suspension.h"
#include "app_wheels.h"
}

int main()
{
    // Setup can.
    app_canTx_init();
    app_canRx_init();

    // Setup apps.
    app_apps_init();

    // Start up state machine and begin ticking.
    app_stateMachine_init(app_mainState_get());
    for (int tick = 0; true; tick += 1)
    {
        if (tick % 1000 == 0)
        {
            app_stateMachine_tick1Hz();
        }

        if (tick % 10 == 0)
        {
            app_stateMachine_tick100Hz();
        }
    }
}