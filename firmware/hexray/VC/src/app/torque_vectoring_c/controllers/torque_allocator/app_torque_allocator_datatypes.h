#include "torque_vectoring_c/datatypes/app_vd_datatypes.h"

typedef struct
{
    double pedal_percentage;
    bool   full_tv_enabled;  // if not full TV we use power limiting and active diff only
    bool   regen_enabled;    // enable regen braking
    bool   vanilla_override; // if this enabled then only use power limiting, power limiting runs always
} TorqueAllocator_inputs;

typedef struct
{
    VD_TorqueToInv torques_to_inverter;
} TorqueAllocator_outputs;