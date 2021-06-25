
#include "App_Motor.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct Motor
{
    void (*set_fake_fund_freq)(double fundamental_freq);
    void (*fake_rotor_enable)(void);
    void (*fake_rotor_disable)(void);
    double (*get_fake_position)(void);
};

struct Motor *App_Motor_Create(
    void (*mtr_set_fake_fund_freq)(double fundamental_freq),
    void (*mtr_fake_rotor_enable)(void),
    void (*mtr_fake_rotor_disable)(void),
    double (*mtr_get_fake_position)(void))
{
    struct Motor *motor = malloc(sizeof(struct Motor));
    assert(motor != NULL);

    motor->set_fake_fund_freq = mtr_set_fake_fund_freq;
    motor->fake_rotor_enable  = mtr_fake_rotor_enable;
    motor->fake_rotor_disable = mtr_fake_rotor_disable;
    motor->get_fake_position  = mtr_get_fake_position;

    return motor;
}

void App_Motor_Destroy(struct Motor *motor)
{
    free(motor);
}

void App_Motor_FakeRotorEnable(struct Motor *motor, double fundamental_freq)
{
    motor->fake_rotor_enable();
    motor->set_fake_fund_freq(fundamental_freq);
}

void App_Motor_FakeRotorDisable(struct Motor *motor)
{
    motor->fake_rotor_disable();
}

void App_Motor_FakeRotorSetFreq(struct Motor *motor, double fundamental_freq)
{
    motor->set_fake_fund_freq(fundamental_freq);
}

double App_Motor_FakeRotorGetPosition(struct Motor *motor)
{
    motor->get_fake_position();
}
