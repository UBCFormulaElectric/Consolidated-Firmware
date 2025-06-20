#include "jobs.h"

#include "app_precharge.h"
#include "app_segments.h"
#include "app_shdnLoop.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "io_bootHandler.h"
#include "io_canTx.h"

#include "io_canTx.h"
#include "io_canRx.h"
// app
#include "app_commitInfo.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "app_stateMachine.h"
// io
#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "app_stateMachine.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_log.h"

void jobs_init()
{
    app_canTx_init();
    app_canRx_init();

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    app_precharge_init();

    app_segments_setDefaultConfig();
    app_segments_initFaults();
    app_segments_balancingInit();

    app_allStates_init();
    app_stateMachine_init(app_initState_get());
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}
