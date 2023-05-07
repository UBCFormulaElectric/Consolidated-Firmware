#include "App_Efuse.h"

struct Efuse {
  void (*enable_channel_0)(void);
  void (*disable_channel_0)(void);
  void (*enable_channel_1)(void);
  void (*disable_channel_1)(void);
  ExitCode (*get_status)(enum Efuse_Status *status);
  ExitCode (*get_channel_0_faults)(enum Efuse_Fault *fault);
  ExitCode (*get_channel_1_faults)(enum Efuse_Fault *fault);
  bool (*is_in_fault_mode)(void);
  bool (*is_in_failsafe_mode)(void);
  void (*delatch_faults)(void);
  float (*get_channel_0_current)(void);
  float (*get_channel_1_current)(void);
};
