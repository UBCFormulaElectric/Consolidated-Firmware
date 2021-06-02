#pragma once

#include "main.h"
#include "controls/App_ControlLoop.h"

void Io_TimerPwmGen_Init(TIM_HandleTypeDef *const tim_handle);
void Io_TimerPwmGen_StartPwm(void);
void Io_TimerPwmGen_StopPwm(void);
void Io_TimerPwmGen_LoadPwm(const struct PhaseValues *phase_pwm_dur);
void Io_TimerPwmGen_SetSwitchingFreq(uint16_t switching_freq);
void Io_TimerPwmGen_SetDeadTime(uint16_t dead_time);
