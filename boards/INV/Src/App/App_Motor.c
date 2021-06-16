//
// Created by ggoodwinwilson on 4/3/2021.
//

#include "App_Motor.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct Motor
{
    void (*adc_cont_mode_init)(void);
    void (*adc_pwm_sync_mode_init)(void);
    void (*adc_start)(void);
};

struct Motor *App_Motor_Create(
        void (*ps_adc_cont_mode_init)(void),
        void (*ps_adc_pwm_sync_mode_init)(void),
        void (*ps_adc_start)(void),
{
    struct Motor *motor = malloc(sizeof(struct Motor));
    assert(motor != NULL);

    motor->adc_cont_mode_init      = ps_adc_cont_mode_init;
    motor->adc_pwm_sync_mode_init  = ps_adc_pwm_sync_mode_init;
    motor->adc_start               = ps_adc_start;

    return motor;
}

void App_Motor_Destroy(struct Motor *motor)
{
    free(motor);
}

void App_Motor_Enable(struct Motor *motor)
{
    motor->adc_stop();
    motor->adc_pwm_sync_mode_init();
    motor->adc_start();
    motor->dac_start();
}

void App_Motor_Disable(struct Motor *motor)
{
    motor->adc_stop();
}

void App_Motor_StandBy(struct Motor *motor)
{
    motor->adc_stop();
    motor->adc_cont_mode_init();
    motor->adc_start();
}

void App_Motor_SetCurrentLimits(
        struct Motor *motor,
        uint32_t           current_lim)
{
    motor->dac_set_current(current_lim);
}

void App_Motor_GetPhaseCurrents(
        struct Motor * motor,
        struct PhaseValues *phase_currents)
{
    motor->get_phase_currents(phase_currents);
}

double App_Motor_GetBusVoltage(struct Motor *motor)
{
    return motor->get_bus_voltage();
}