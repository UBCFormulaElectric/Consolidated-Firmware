#pragma once

#include "util_errorCodes.hpp"

result<void> vicor_operation(bool enable);
result<void> vicor_clearFaults();
result<void> vicor_readVin(float *val);
result<void> vicor_readIin(float *val);
result<void> vicor_readVout(float *val);
result<void> vicor_readIout(float *val);
result<void> vicor_readTemp(float *val);
result<void> vicor_readPout(float *val);
result<void> vicor_readSerial(void);
result<void> vicor_statusWord(uint16_t *status);
result<void> vicor_statusIout(uint8_t *status);
result<void> vicor_statusInput(uint8_t *status);
result<void> vicor_statusTemp(uint8_t *status);
result<void> vicor_statusComm(uint8_t *status);
result<void> vicor_statusMfrSpecific(uint8_t *status);
