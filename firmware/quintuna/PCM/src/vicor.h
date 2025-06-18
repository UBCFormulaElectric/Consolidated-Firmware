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
ExitCode vicor_setpage(bool enabled);
ExitCode vicor_readSerial(void);
ExitCode vicor_statusWord(uint16_t *status);
ExitCode vicor_statusIout(uint8_t *status);
ExitCode vicor_statusInput(uint8_t *status);
ExitCode vicor_statusTemp(uint8_t *status);
ExitCode vicor_statusComm(uint8_t *status);
ExitCode vicor_statusMfrSpecific(uint8_t *status);
