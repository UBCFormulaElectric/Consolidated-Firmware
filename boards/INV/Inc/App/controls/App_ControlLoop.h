#pragma once

#include "App_InvWorld.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

typedef struct DqsValues
{
    double s;
    double d;
    double q;
}DqsValues;

struct PhaseValues
{
    double a;
    double b;
    double c;
};

typedef struct ControllerValues
{
    double prev_integral_input;
    double integral_sum;
    double gain;
    double time_const;
} ControllerValues;

enum Mode
{
    GEN_SINE_I,
    GEN_SINE_M,
    MOTOR_CONTROL,
};

struct InvWorld;

void App_ControlLoop_Run(
    double                 rotor_speed_ref,
    uint8_t                mode,
    const struct InvWorld *world,
    double                 mod_index_request,
    double                 fund_freq_request);
