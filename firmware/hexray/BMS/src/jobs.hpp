#pragma once
#include "io_semaphore.hpp"

extern io::semaphore spi_bus_lock;

void jobs_init();
void jobs_run1Hz_tick();
void jobs_run100Hz_tick();
void jobs_run1kHz_tick();
void jobs_runAdbmsVoltages_tick();
void jobs_runAdbmsConfigs_tick();
void jobs_runAdbmsTemperatures_tick();
void jobs_runAdbmsDiagnostics_tick();
