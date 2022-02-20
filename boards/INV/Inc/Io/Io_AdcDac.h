#pragma once

#include "control/App_ControlLoop.h"

#define CUR_SNS_GAIN 100.0f // Allegro phase current sensor gain, in A/V
#define MAX_CUR_ADC_OFFSET \
    0.025f // 25mV, the max offset error for Allegro ACS758ECB-200B-PSS-T

void Io_AdcDac_AdcContModeInit(void);
void Io_AdcDac_AdcPwmSyncModeInit(void);
void Io_AdcDac_AdcStart(void);
void Io_AdcDac_AdcStop(void);

void                      Io_AdcDac_DacStart(void);
void                      Io_AdcDac_DacSetCurrentLim(float current);
const struct PhaseValues *Io_AdcDac_CorrectOffset(void);
void  Io_AdcDac_GetPhaseCurrents(struct PhaseValues *const phase_currents);
float Io_AdcDac_GetGpioVal(void);
float Io_AdcDac_GetBusVoltage(void);
float Io_AdcDac_GetMotorTemp(void);
float Io_AdcDac_ReadMotorTemp(void);
float Io_AdcDac_GetPowerstageTemp(void);
float Io_AdcDac_ReadPowerstageTemp(void);
float Io_AdcDac_Dac1GetVoltage(void);
float Io_AdcDac_Dac2GetVoltage(void);
float Io_AdcDac_Dac1GetCurrentLim(void);
float Io_AdcDac_Dac2GetCurrentLim(void);
