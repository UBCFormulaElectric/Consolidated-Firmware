#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CZMQ for sockets.
#include <czmq.h>

// Fake io Layer.
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"
#include "fake_io_loadCell.hpp"

// SIL board functions.
extern "C"
{
#include "sil.h"
}

// IO layer.
extern "C"
{
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

void tasks_init()
{
    // Setup can with fake callback for transmission.
    io_canTx_init(sil_txCallback);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();

    // Setup apps.
    app_apps_init();

    // Start up state machine.
    app_stateMachine_init(app_mainState_get());
}

void tasks_1kHz(uint32_t time_ms)
{
    io_canTx_enqueueOtherPeriodicMsgs(time_ms);
}

void tasks_100Hz(uint32_t time_ms)
{
    app_stateMachine_tick100Hz();
    io_canTx_enqueue100HzMsgs();
}

void tasks_1Hz(uint32_t time_ms)
{
    app_stateMachine_tick1Hz();
    io_canTx_enqueue1HzMsgs();
}

int main(int argc, char *argv[])
{
    sil_main(argc, argv, tasks_init, tasks_1Hz, tasks_100Hz, tasks_1kHz);
}