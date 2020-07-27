#pragma once

#include <stdbool.h>
#include "App_SharedExitCode.h"

// clang-format off
// Enumeration for the status types
enum Efuse_Status
{
    StatusType_NoFault           = (0 << 0), // No fault
    StatusType_Out0Enabled       = (1 << 0), // Channel 0 output is on
    StatusType_Out1Enabled       = (1 << 1), // Channel 0 output is on
    StatusType_Channel0Fault     = (1 << 2), // Fault on channel 0
    StatusType_Channel1Fault     = (1 << 3), // Fault on channel 1
    StatusType_Channel0RetryFull = (1 << 4), // Channel 0 auto-retry counter full
    StatusType_Channel1RetryFull = (1 << 5), // Channel 1 auto-retry counter full
    StatusType_PowerOnReset      = (1 << 6), // Power-on reset has occurred
    StatusType_UnderVoltage      = (1 << 7), // Under voltage fault
    StatusType_OverVoltage       = (1 << 8)  // Over voltage fault
};

// Enumeration for the fault types
enum Efuse_Fault
{
    EFUSE_NO_FAULT          = (0 << 0), // No fault
    EFUSE_OVERCURRENT      = (1 << 0), // Over current fault
    EFUSE_SHORT_CIRCUIT     = (1 << 1), // Severe short circuit fault
    EFUSE_OVERTEMP         = (1 << 2), // Over temperature fault
    EFUSE_OUTPUT_SHORTED    = (1 << 3), // Output shorted to Vpwr fault
    EFUSE_OPEN_LOAD_OFF_STATE = (1 << 4), // Open load detected in off state fault
    EFUSE_OPEN_LOAD_ON_STATE  = (1 << 5), // Open load detected in on state fault
    EFUSE_OVERTEMP_WARNING  = (1 << 8)  // Over temperature warning fault
};
// clang-format on

struct Efuse
{
    ExitCode (*configure_efuse)(void);
    void (*enable_channel0)(void);
    void (*disable_channel0)(void);
    void (*enable_channel1)(void);
    void (*disable_channel1)(void);
    ExitCode (*get_status)(enum Efuse_Status *status);
    ExitCode (*get_channel0_faults)(enum Efuse_Fault *fault);
    ExitCode (*get_channel1_faults)(enum Efuse_Fault *fault);
    bool (*is_in_fault_mode)(void);
    bool (*is_in_failsafe_mode)(void);
    void (*delatch_faults)(void);
    bool (*get_channel0_current)(float *channel0_current);
    bool (*get_channel1_current)(float *channel1_current);
};
