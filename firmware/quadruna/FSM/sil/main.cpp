#include <stdio.h>
#include <stdlib.h>

// Fake io Layer.
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"
#include "fake_io_loadCell.hpp"

extern "C" {
#include "io_canRx.h"
#include "io_canTx.h"
}

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

void io_fakeCan_txCallback(const JsonCanMsg *msg) {
    printf("can tx:\n");
    printf("\tid: %d\n", msg->std_id);

    printf("\tdata: ");
    for (int i = 8; i > 0; i -= 1) {
        printf("%2x ", msg->data[i]);
    }

    printf("\n");
}

void io_fakeCan_rx(JsonCanMsg *msg) {
    io_canRx_updateRxTableWithMessage(msg);
}

int main()
{
    // Setup can with fake callback for transmission.
    io_canTx_init(io_fakeCan_txCallback);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();

    // Setup apps.
    app_apps_init();

    // Start up state machine and begin ticking.
    app_stateMachine_init(app_mainState_get());

    for (int time_ms = 0; true; time_ms += 1)
    {
        io_canTx_enqueueOtherPeriodicMsgs(time_ms);

        if (time_ms % 1000 == 0)
        {
            app_stateMachine_tick1Hz();
            io_canTx_enqueue1HzMsgs();
        }

        if (time_ms % 10 == 0)
        {
            app_stateMachine_tick100Hz();
            io_canTx_enqueue100HzMsgs();
        }
    }
}