#pragma once

#include <stdbool.h>

namespace io::vcShdn
{
bool TsmsFault_get(void);
bool RearRightMotorInterlock_get(void);
bool MSDOrEMeterFault_get(void);
}; // namespace io::vcShdn

