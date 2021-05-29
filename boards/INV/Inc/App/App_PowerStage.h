#pragma once

#include "controls/App_ControlLoop.h"

struct PowerStage;
struct PhaseValues;

struct PowerStage *App_PowerStage_Create(
        void (*ps_adc_cont_mode_init)(void),
        void (*ps_adc_pwm_sync_mode_init)(void),
        void (*ps_adc_start)(void),
        void (*ps_adc_stop)(void),
        void (*ps_dac_start)(void),
        void (*ps_dac_set_current)(double current),
        void (*ps_get_phase_currents)(struct PhaseValues * phase_currents),
        double (*ps_get_bus_voltage)(void),
        double (*ps_get_powerstage_temp)(void),
        bool (*ps_get_pha_oc_fault)(void),
        bool (*ps_get_phb_oc_fault)(void),
        bool (*ps_get_phc_oc_fault)(void),
        bool (*ps_get_powerstage_ot_fault)(void));
void App_PowerStage_Destroy(struct PowerStage *power_stage);
void App_PowerStage_Enable(struct PowerStage *power_stage);
void App_PowerStage_Disable(struct PowerStage *power_stage);
void App_PowerStage_StandBy(struct PowerStage *power_stage);
void App_PowerStage_SetCurrentLimits(struct PowerStage *power_stage, uint32_t current_lim);
void App_PowerStage_GetPhaseCurrents(struct PowerStage *power_stage, struct PhaseValues *phase_currents);
double App_PowerStage_GetBusVoltage(struct PowerStage *power_stage);
double App_PowerStage_GetTemperature(struct PowerStage *power_stage);
bool App_PowerStage_GetPhaOCFault(struct PowerStage *power_stage);
bool App_PowerStage_GetPhbOCFault(struct PowerStage *power_stage);
bool App_PowerStage_GetPhcOCFault(struct PowerStage *power_stage);
bool App_PowerStage_GetPowerStageOTFault(struct PowerStage *power_stage);
