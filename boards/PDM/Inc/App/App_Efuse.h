#pragma once

#include <stdbool.h>

#include "App_SharedExitCode.h"

// clang-format off
// Enumeration for the status types
enum Efuse_Status
{
    EFUSE_NO_FAULTS           = (0 << 0), // No faults on either channel
    EFUSE_OUT0_ENABLED        = (1 << 0), // Channel 0 output is enabled
    EFUSE_OUT1_ENABLED        = (1 << 1), // Channel 1 output is enabled
    EFUSE_CHANNEL0_FAULT      = (1 << 2), // Fault on channel 0
    EFUSE_CHANNEL1_FAULT      = (1 << 3), // Fault on channel 1
    EFUSE_CHANNEL0_RETRY_FULL = (1 << 4), // Channel 0 auto-retry counter full
    EFUSE_CHANNEL1_RETRY_FULL = (1 << 5), // Channel 1 auto-retry counter full
    EFUSE_POWER_ON_RESET      = (1 << 6), // Power-on reset has occurred
    EFUSE_UNDERVOLTAGE        = (1 << 7), // Under voltage fault
    EFUSE_OVERVOLTAGE         = (1 << 8)  // Over voltage fault
};

// Enumeration for the fault types
enum Efuse_Fault
{
    EFUSE_NO_FAULT            = (0 << 0), // No fault
    EFUSE_OVERCURRENT         = (1 << 0), // Over current fault
    EFUSE_SHORT_CIRCUIT       = (1 << 1), // Severe short circuit fault
    EFUSE_OVERTEMP            = (1 << 2), // Over temperature fault
    EFUSE_OUTPUT_SHORTED      = (1 << 3), // Output shorted to Vpwr fault
    EFUSE_OPEN_LOAD_OFF_STATE = (1 << 4), // Open load detected in off state fault
    EFUSE_OPEN_LOAD_ON_STATE  = (1 << 5), // Open load detected in on state fault
    EFUSE_OVERTEMP_WARNING    = (1 << 8)  // Over temperature warning fault
};
// clang-format on

struct Efuse;
