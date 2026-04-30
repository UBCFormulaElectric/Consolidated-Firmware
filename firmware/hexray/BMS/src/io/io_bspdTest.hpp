#pragma once

namespace io::bspdtest
{
void enable(const bool enable);
bool isCurrentThresholdExceeded(void);
bool isBrakePressureThresholdExceeded(void);
bool isAccelBrakeOk(void);
} // namespace io::bspdtest