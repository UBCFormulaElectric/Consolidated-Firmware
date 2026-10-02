#pragma once

void jobs_init(void);
void jobs_initImu();
void jobs_run1Hz_tick(void);
void jobs_run100Hz_tick(void);
void jobs_run1kHz_tick(void);
void jobs_runChimera_tick(void);
void jobs_runImu_tick();
void jobs_runBatteryMonitoring_tick(void);
void jobs_runPowerMonitoring_tick(void);
void jobs_runSbgEllipse_tick(void);
