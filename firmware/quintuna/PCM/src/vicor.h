#pragma once

#include "app_utils.h"
#include <stdbool.h>

ExitCode vicor_operation(bool enable);
ExitCode vicor_clearFaults(void);
ExitCode vicor_readVin(float *val);
ExitCode vicor_readIin(float *val);
ExitCode vicor_readVout(float *val);
ExitCode vicor_readIout(float *val);
ExitCode vicor_readTemp(float *val);
ExitCode vicor_readPout(float *val);
