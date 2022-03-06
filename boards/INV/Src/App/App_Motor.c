
#include "App_Motor.h"
#include "Io_ECI1118.h"
#include "Io_AdcDac.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/*
struct Motor
{
    void (*set_fake_fund_freq)(float fundamental_freq);
    void (*fake_rotor_enable)(void);
    void (*fake_rotor_disable)(void);
    float (*get_fake_position)(void);
};

struct Motor *App_Motor_Create(
    void (*mtr_set_fake_fund_freq)(float fundamental_freq),
    void (*mtr_fake_rotor_enable)(void),
    void (*mtr_fake_rotor_disable)(void),
    float (*mtr_get_fake_position)(void))
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
*/

bool App_Motor_GetOTFaultStatus(void)
{
    return Io_ECI1118_GetMotorOTFault();
}

float App_Motor_GetPositionBlocking(void)
{
    return Io_ECI1118_GetPositionBlocking();
}

void App_Motor_StartGetPosition(void)
{
    Io_ECI1118_StartGetPosition();
}

float App_Motor_GetPosition(void)
{
    return Io_ECI1118_ReadPosition();
}

bool App_Motor_GetOTAlarm(void)
{
    //TODO change this back when using motor
    return 0;//!HAL_GPIO_ReadPin(nMOTOR_OT_ALARM_GPIO_Port, nMOTOR_OT_ALARM_Pin);
}

float App_Motor_GetTemperature(void)
{
    return Io_AdcDac_GetMotorTemp();
}

float App_Motor_GetDeratedCurrent(void)
{
    float derated_current = 0;
    float motor_temp      = Io_AdcDac_GetMotorTemp();
    if (motor_temp < 50.0f)
    {
        derated_current = 53.1f;
    }
    else if (motor_temp >= 50.0f && motor_temp <= 150.0f)
    {
        derated_current = -0.531f * motor_temp + 79.65f;
    }
    else if (motor_temp > 150.0f)
    {
        derated_current = 0;
    }

    return derated_current;
}
