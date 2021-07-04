#pragma once

#include "App_InvWorld.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

struct DqsValues
{
    float s;
    float d;
    float q;
};

struct PhaseValues
{
    float a;
    float b;
    float c;
};

struct ControllerValues
{
    float prev_integral_input;
    float integral_sum;
    float gain;
    float time_const;
};

enum Mode
{
    GEN_SINE_I,
    GEN_SINE_M,
    MOTOR_CONTROL,
};

struct InvWorld;

void App_ControlLoop_Run(
    float                  rotor_speed_ref,
    uint8_t                mode,
    const struct InvWorld *world,
    float                  mod_index_request,
    float                  ph_cur_rms_request);
