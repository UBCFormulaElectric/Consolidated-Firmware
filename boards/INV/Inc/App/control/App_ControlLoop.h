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
    float prev_error;
    float integral_sum;
    float output;
    float gain;
    float time_const;
};

enum Mode
{
    MODE_UNDEFINED,
    GEN_SINE_M,
    GEN_SINE_I,
    MOTOR_CONTROL,
};

enum
{
    TORQUE_CONTROL,
    SPEED_CONTROL,
};

struct InvWorld;

void App_ControlLoop_Run(const struct InvWorld *world);

void App_ControlLoop_GetIqControllerValues(struct ControllerValues *controller);
void App_ControlLoop_GetIdControllerValues(struct ControllerValues *controller);
void App_ControlLoop_GetSpeedControllerValues(
    struct ControllerValues *controller);
float   App_ControlLoop_GetLineToNeutralVoltage(void);
uint8_t App_ControlLoop_GetMode(void);
float   App_ControlLoop_GetModIndex(void);
float   App_ControlLoop_GetRotorPosition(void);
float   App_ControlLoop_GetRotorSpeed(void);
float   App_ControlLoop_GetPhcCurCalc(void);
bool    App_ControlLoop_GetFwFlag(void);
void    App_ControlLoop_GetDqsRefCurrents(struct DqsValues *dqs_values);
void    App_ControlLoop_GetDqsActualCurrents(struct DqsValues *dqs_values);
float   App_ControlLoop_GetPowerStageCurrentLimit(void);
float   App_ControlLoop_GetMotorCurrentLimit(void);
float   App_ControlLoop_GetCurrentLimit(void);
float App_ControlLoop_SinLookup(float rad);
float App_ControlLoop_CosLookup(float rad);
void App_ControlLoop_SinCosLutInit(void);
