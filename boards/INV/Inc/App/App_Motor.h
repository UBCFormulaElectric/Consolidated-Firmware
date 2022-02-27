#pragma once
#include <stdint.h>
#include <stdbool.h>
/*
struct Motor;

struct Motor *App_Motor_Create(
    void (*mtr_set_fake_fund_freq)(float fundamental_freq),
    void (*mtr_fake_rotor_enable)(void),
    void (*mtr_fake_rotor_disable)(void),
    float (*mtr_get_fake_position)(void));
void  App_Motor_Destroy(struct Motor *motor);
void  App_Motor_FakeRotorEnable(struct Motor *motor, float fundamental_freq);
void  App_Motor_FakeRotorDisable(struct Motor *motor);
void  App_Motor_FakeRotorSetFreq(struct Motor *motor, float fundamental_freq);
float App_Motor_FakeRotorGetPosition(struct Motor *motor);
 */
float App_Motor_GetPositionBlocking(void);
void  App_Motor_StartGetPosition(void);
float App_Motor_GetPosition(void);
bool  App_Motor_GetOTAlarm(void);
float App_Motor_GetTemperature(void);
float App_Motor_GetDeratedCurrent(void);
