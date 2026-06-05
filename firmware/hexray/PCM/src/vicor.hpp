#pragma once

#include "util_errorCodes.hpp"

result<void>     vicor_operation(bool enable);
result<void>     vicor_read_operation();
result<void>     vicor_clearFaults();
result<float>    vicor_readVin();
result<float>    vicor_readIin();
result<float>    vicor_readVout();
result<float>    vicor_readIout();
result<float>    vicor_readTemp();
result<float>    vicor_readPout();
result<void>     vicor_readSerial();
result<uint16_t> vicor_statusWord();
result<uint8_t>  vicor_statusIout();
result<uint8_t>  vicor_statusInput();
result<uint8_t>  vicor_statusTemp();
result<uint8_t>  vicor_statusComm();
result<uint8_t>  vicor_statusMfrSpecific();
