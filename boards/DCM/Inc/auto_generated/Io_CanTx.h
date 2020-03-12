#pragma once

#include <stdint.h>
#include "auto_generated/CanMsgs.h"

struct CanTxInterface;

void Io_CanTx_TransmitPeriodicMessages(
    struct CanTxInterface* can_tx_interface,
    const uint32_t current_tick);

void Io_CanTx_EnqueueNonPeriodicMsg_DCM_STARTUP(struct CanMsgs_dcm_startup_t* payload);
void Io_CanTx_EnqueueNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanMsgs_dcm_watchdog_timeout_t * payload);

