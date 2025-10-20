#include "jobs.h"
#include "app_utils.h"
#include "io_canMsg.h"
#include "io_canQueue.h"
#include "io_canQueues.h"
#include "app_jsoncan.h"
#include <app_canTx.h>
#include <io_canTx.h>
#include <stdbool.h>
#include "io_time.h"
#include "app_canRx.h"
#include <app_commitInfo.h>
#include <io_canReroute.h>
#include "io_bootloaderReroute.h"
#include <string.h>

static void fd_can_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&fd_can_tx_queue, &msg);
}

static void sx_can_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&sx_can_tx_queue, &msg);
}

static void inv_can_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&inv_can_tx_queue, &msg);
}

void jobs_init()
{
    app_canTx_init();
    app_canRx_init();

    io_canTx_init(fd_can_tx, sx_can_tx, inv_can_tx);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    io_canTx_enableMode_can3(CAN3_MODE_DEFAULT, true);

    io_canQueue_initTx(&fd_can_tx_queue);
    io_canQueue_initTx(&sx_can_tx_queue);
    io_canQueue_initTx(&inv_can_tx_queue);
    io_canReroute_init(fd_can_tx, sx_can_tx, inv_can_tx);

    app_canTx_VCR_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VCR_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_VCR_Heartbeat_set(true);
}