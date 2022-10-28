#include <assert.h>

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SharedSoftwareWatchdog.h"
#include "Io_SharedCan.h"
#include "Io_SharedErrorTable.h"
#include "Io_SharedHardFaultHandler.h"
#include "Io_StackWaterMark.h"
#include "Io_SoftwareWatchdog.h"

#include "App_GsmWorld.h"
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "states/App_DefaultState.h"

static struct GsmWorld *         world;
static struct StateMachine *     state_machine;
static struct GsmCanTxInterface *can_tx;
static struct GsmCanRxInterface *can_rx;
static struct ErrorTable *       error_table;

void App_Main_InitIo()
{
    Io_SharedHardFaultHandler_Init();
    Io_StackWaterMark_Init(can_tx);
    Io_SharedSoftwareWatchdog_Init(Io_HardwareWatchdog_Refresh, Io_SoftwareWatchdog_TimeoutCallback);
    Io_SoftwareWatchdog_Init(can_tx);
}

void App_Main_InitApp()
{
    // Init app modules
    can_tx = App_CanTx_Create(
        Io_CanTx_EnqueueNonPeriodicMsg_GSM_STARTUP, Io_CanTx_EnqueueNonPeriodicMsg_GSM_WATCHDOG_TIMEOUT);
    can_rx = App_CanRx_Create();
    //    error_table = App_SharedErrorTable_Create();

    // Create world and state machine
    world         = App_GsmWorld_Create(can_tx, can_rx);
    state_machine = App_SharedStateMachine_Create(world, App_GetDefaultState());

    // Send startup msg
    struct CanMsgs_gsm_startup_t payload = { .dummy = 0 };
    App_CanTx_SendNonPeriodicMsg_GSM_STARTUP(can_tx, &payload);
}

void App_Main_InitTask1Hz(void);
void App_Main_InitTask100Hz(void);
void App_Main_InitTask1kHz(void);
void App_Main_InitTaskCanRx(void);
void App_Main_InitTaskCanTx(void);

void App_Main_InitTask1Hz(void);
void App_Main_InitTask100Hz(void);
void App_Main_InitTask1kHz(void);
void App_Main_InitTaskCanRx(void);
void App_Main_InitTaskCanTx(void);