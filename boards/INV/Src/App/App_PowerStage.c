//
// Created by ggoodwinwilson on 4/3/2021.
//

#include "App_PowerStage.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct PowerStage
{
    void (*adc_cont_mode_init)(void);
    void (*adc_pwm_sync_mode_init)(void);
    void (*adc_start)(void);
    void (*adc_stop)(void);
    void (*dac_start)(void);
    void (*dac_set_current)(float current);
    void (*get_phase_currents)(struct PhaseValues *const phase_currents);
    const struct PhaseValues *(*correct_current_offset)(void);
    float (*get_bus_voltage)(void);
    float (*get_powerstage_temp)(void);
    bool (*get_pha_oc_fault)(void);
    bool (*get_phb_oc_fault)(void);
    bool (*get_phc_oc_fault)(void);
    bool (*get_powerstage_ot_fault)(void);
};

struct PowerStage *App_PowerStage_Create(
    void (*ps_adc_cont_mode_init)(void),
    void (*ps_adc_pwm_sync_mode_init)(void),
    void (*ps_adc_start)(void),
    void (*ps_adc_stop)(void),
    void (*ps_dac_start)(void),
    void (*ps_dac_set_current)(float current),
    void (*ps_get_phase_currents)(struct PhaseValues *const phase_currents),
    const struct PhaseValues *(*ps_correct_current_offset)(void),
    float (*ps_get_bus_voltage)(void),
    float (*ps_get_powerstage_temp)(void),
    bool (*ps_get_pha_oc_fault)(void),
    bool (*ps_get_phb_oc_fault)(void),
    bool (*ps_get_phc_oc_fault)(void),
    bool (*ps_get_powerstage_ot_fault)(void))
{
    struct PowerStage *power_stage = malloc(sizeof(struct PowerStage));
    assert(power_stage != NULL);

    power_stage->adc_cont_mode_init      = ps_adc_cont_mode_init;
    power_stage->adc_pwm_sync_mode_init  = ps_adc_pwm_sync_mode_init;
    power_stage->adc_start               = ps_adc_start;
    power_stage->adc_stop                = ps_adc_stop;
    power_stage->dac_start               = ps_dac_start;
    power_stage->dac_set_current         = ps_dac_set_current;
    power_stage->get_phase_currents      = ps_get_phase_currents;
    power_stage->correct_current_offset  = ps_correct_current_offset;
    power_stage->get_bus_voltage         = ps_get_bus_voltage;
    power_stage->get_powerstage_temp     = ps_get_powerstage_temp;
    power_stage->get_pha_oc_fault        = ps_get_pha_oc_fault;
    power_stage->get_phb_oc_fault        = ps_get_phb_oc_fault;
    power_stage->get_phc_oc_fault        = ps_get_phc_oc_fault;
    power_stage->get_powerstage_ot_fault = ps_get_powerstage_ot_fault;

    return power_stage;
}

void App_PowerStage_Destroy(struct PowerStage *power_stage)
{
    free(power_stage);
}

void App_PowerStage_Enable(struct PowerStage *power_stage)
{
    power_stage->adc_stop();
    power_stage->adc_pwm_sync_mode_init();
    power_stage->adc_start();
    power_stage->dac_start();
}

void App_PowerStage_Disable(struct PowerStage *power_stage)
{
    power_stage->adc_stop();
    power_stage->adc_cont_mode_init();
    power_stage->adc_start();
}

void App_PowerStage_StandBy(struct PowerStage *power_stage)
{
    power_stage->adc_stop();
    power_stage->adc_cont_mode_init();
    power_stage->adc_start();
    power_stage->dac_start();
}

void App_PowerStage_SetCurrentLimits(
    struct PowerStage *power_stage,
    float              current_lim)
{
    power_stage->dac_set_current(current_lim);
}

void App_PowerStage_GetPhaseCurrents(
    struct PowerStage * power_stage,
    struct PhaseValues *phase_currents)
{
    power_stage->get_phase_currents(phase_currents);
}

const struct PhaseValues *
    App_PowerStage_CorrectCurrentOffset(struct PowerStage *power_stage)
{
    return power_stage->correct_current_offset();
}

float App_PowerStage_GetBusVoltage(struct PowerStage *power_stage)
{
    return power_stage->get_bus_voltage();
}

float App_PowerStage_GetTemperature(struct PowerStage *power_stage)
{
    return power_stage->get_powerstage_temp();
}

bool App_PowerStage_GetPhaOCFault(struct PowerStage *power_stage)
{
    return power_stage->get_pha_oc_fault();
}

bool App_PowerStage_GetPhbOCFault(struct PowerStage *power_stage)
{
    return power_stage->get_phb_oc_fault();
}

bool App_PowerStage_GetPhcOCFault(struct PowerStage *power_stage)
{
    return power_stage->get_phc_oc_fault();
}

bool App_PowerStage_GetPowerStageOTFault(struct PowerStage *power_stage)
{
    return power_stage->get_powerstage_ot_fault();
}
