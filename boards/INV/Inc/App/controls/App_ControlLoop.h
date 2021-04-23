#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "math.h"

typedef struct
{
    double s;
    double d;
    double q;
} DqsValues;

typedef struct
{
    double a;
    double b;
    double c;
} PhaseValues;

typedef struct
{
    double prev_integral_input;
    double integral_sum;
    double gain;
    double time_const;
} ControllerValues;

void controlLoop(double torque_ref, double rotor_speed_ref);
