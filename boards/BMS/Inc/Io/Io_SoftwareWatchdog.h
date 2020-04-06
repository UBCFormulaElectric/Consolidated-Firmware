#pragma once

#include "auto_generated/App_CanTx.h"
#include "Io_SharedSoftwareWatchdog.h"

/**
 * Init the software watchdog
 * @param can_tx An interface for transmitting CAN messages
 */
void Io_SoftwareWatchdog_Init(struct BMSCanTxInterface *can_tx);

void Io_HardwareWatchdog_Refresh(void);
void Io_SoftwareWatchdog_TimeoutCallback(
    SoftwareWatchdogHandle_t sw_watchdog_handle);
