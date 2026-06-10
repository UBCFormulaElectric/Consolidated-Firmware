#pragma once

void jobs_init();
void jobs_run1Hz_tick();
void jobs_run100Hz_tick();
void jobs_run1kHz_tick();
void jobs_runAdbmsVoltages_tick();
void jobs_runAdbmsConfigs_tick();
void jobs_runAdbmsAux_tick();
void jobs_runAdbmsCellOwc_tick();

// Kick the ADBMS config task to sync ASAP instead of waiting for its next periodic
// tick. The config task runs the on-demand pass at its normal priority.
//   - jobs_requestConfigSync():        request only, returns immediately.
//   - jobs_requestConfigSyncAndWait(): request, then block until a sync that
//                                      started after this call completes.
void jobs_requestConfigSync();
void jobs_requestConfigSyncAndWait();
