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
    float output;
    float gain;
    float time_const;
};

struct MotorControlFaults
{
    bool torque_request_implausible;
    bool rotor_position_implausible;
};

enum Mode
{
    GEN_SINE_I,
    GEN_SINE_M,
    MOTOR_CONTROL,
    MODE_UNDEFINED,
};

struct InvWorld;

void App_ControlLoop_Run(
    float                  rotor_speed_ref,
    uint8_t                mode,
    const struct InvWorld *world,
    float                  mod_index_request,
    float                  ph_cur_rms_request);

void App_ControlLoop_GetIqControllerValues(struct ControllerValues* controller);
void App_ControlLoop_GetIdControllerValues(struct ControllerValues* controller);
void App_ControlLoop_GetSpeedControllerValues(struct ControllerValues* controller);
void App_ControlLoop_GetFaults(struct MotorControlFaults* faults);
uint8_t App_ControlLoop_GetMode(void);
float App_ControlLoop_GetModIndex(void);
float App_ControlLoop_GetRotorSpeed(void);
float App_ControlLoop_GetPhcCurCalc(void);
bool App_ControlLoop_GetFwFlag(void);
