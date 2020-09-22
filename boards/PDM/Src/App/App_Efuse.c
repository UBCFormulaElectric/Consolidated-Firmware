#include "App_Efuse.h"

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
