#pragma once

#include "controls/App_ControlLoop.h"

struct DqsValues generateRefCurrents(
    const struct DqsValues *const dqs_ref_currents,
    float                         omega,
    float                         vdc_sensor_val,
    bool *                        fw_flag);
