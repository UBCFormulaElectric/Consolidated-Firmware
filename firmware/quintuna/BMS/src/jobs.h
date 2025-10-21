/**
 * This file is meant to help unit testing.
 */

#pragma once

#include "io/canMsg.h"
#include "io/canQueue.h"

extern CanTxQueue can_tx_queue;

/**
 * Anything that needs to be initialized for unit testing must be put here
 */
void jobs_init(void);

/**
 * Anything that needs to run on periodic intervals in the unit test is to be placed here
 */
void jobs_run1Hz_tick(void);
void jobs_run100Hz_tick(void);
void jobs_run1kHz_tick(void);

void jobs_initLTCVoltages(void);
void jobs_runLTCVoltages(void);
void jobs_initLTCTemps(void);
void jobs_runLTCTemperatures(void);
void jobs_initLTCDiagnostics(void);
void jobs_runLTCDiagnostics(void);
