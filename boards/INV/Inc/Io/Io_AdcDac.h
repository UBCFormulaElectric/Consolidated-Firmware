#pragma once

#include "controls/App_ControlLoop.h"

void Io_AdcDac_AdcContModeInit(void);
void Io_AdcDac_AdcPwmSyncModeInit(void);
void Io_AdcDac_AdcStart(void);
void Io_AdcDac_AdcStop(void);

void Io_AdcDac_DacStart(void);
void Io_AdcDac_DacSetCurrent(float current);

void  Io_AdcDac_GetPhaseCurrents(struct PhaseValues *const phase_currents);
float Io_AdcDac_GetGpioVal(void);
float Io_AdcDac_GetBusVoltage(void);
float Io_AdcDac_GetMotorTemp(void);
float Io_AdcDac_GetPowerstageTemp(void);
