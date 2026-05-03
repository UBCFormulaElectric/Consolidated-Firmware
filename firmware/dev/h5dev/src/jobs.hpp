#pragma once

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
void jobs_runCanTx_tick(void);
void jobs_runCanRx_tick(void);
void jobs_runChimera_tick(void);
