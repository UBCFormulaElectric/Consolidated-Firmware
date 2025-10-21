#pragma once

#include "io/canQueue.h"

extern CanTxQueue can_tx_queue;

void jobs_init();
void jobs_run1Hz_tick();
void jobs_run100Hz_tick();
void jobs_run1kHz_tick();
