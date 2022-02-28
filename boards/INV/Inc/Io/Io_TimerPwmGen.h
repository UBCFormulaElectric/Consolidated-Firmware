#pragma once

#include "main.h"
#include "control/App_ControlLoop.h"

void Io_TimerPwmGen_StartPwmTimer(void);
void Io_TimerPwmGen_StopPwm(void);
void Io_TimerPwmGen_LoadPwm(const struct PhaseValues *phase_pwm_dur);
void Io_TimerPwmGen_SetSwitchingFreq(uint16_t switching_freq);
void Io_TimerPwmGen_SetDeadTime(uint16_t dead_time);
