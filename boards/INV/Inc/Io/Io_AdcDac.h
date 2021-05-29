#pragma once

#include "main.h"
#include "controls/App_ControlLoop.h"

void Io_AdcDac_Init(
    ADC_HandleTypeDef *const adc1,
    ADC_HandleTypeDef *const adc2,
    DAC_HandleTypeDef *const dac);

void Io_AdcDac_AdcContModeInit(void);
void Io_AdcDac_AdcPwmSyncModeInit(void);
void Io_AdcDac_AdcStart(void);
void Io_AdcDac_AdcStop(void);

void Io_AdcDac_DacStart(void);
void Io_AdcDac_DacSetCurrent(double current);

void Io_AdcDac_GetPhaseCurrents(struct PhaseValues *const phase_currents);
double Io_AdcDac_GetGpioVal(void);
double Io_AdcDac_GetBusVoltage(void);
double Io_AdcDac_GetMotorTemp(void);
double Io_AdcDac_GetPowerstageTemp(void);
