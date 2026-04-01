#pragma once

void jobs_init(void);
void jobs_adbms_init(void);
void jobs_run1Hz_tick(void);
void jobs_run100Hz_tick(void);
void jobs_run1kHz_tick(void);
void jobs_runCanTx_tick(void);
void jobs_runCanRx_tick(void);
void jobs_runAdbmsVoltages_tick(void);
void jobs_runAdbmsFilteredVoltages_tick(void);
void jobs_runAdbmsTemperatures_tick(void);
void jobs_runAdbmsDiagnostics_tick(void);
