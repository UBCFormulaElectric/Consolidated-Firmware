#pragma once

void jobs_init(void);
void jobs_run1Hz_tick(void);
void jobs_run100Hz_tick(void);
void jobs_run1kHz_tick(void);
void jobs_runCanTx(void);
void jobs_runCaRx(void);
void jobs_runChimera_tick(void);
void jobs_initLTCVoltages(void);
void jobs_runLTCVoltages(void);
void jobs_initLTCTemps(void);
