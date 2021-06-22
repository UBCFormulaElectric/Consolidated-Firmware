#pragma once

struct Motor;

struct Motor *App_Motor_Create(
        void (*mtr_set_fake_fund_freq)(double fundamental_freq),
        void (*mtr_fake_rotor_enable)(void),
        void (*mtr_fake_rotor_disable)(void),
        double (*mtr_get_fake_position)(void));
void App_Motor_Destroy(struct Motor *motor);
void App_Motor_FakeRotorEnable(struct Motor *motor, double fundamental_freq);
void App_Motor_FakeRotorDisable(struct Motor *motor);
void App_Motor_FakeRotorSetFreq(struct Motor *motor, double fundamental_freq);
double App_Motor_FakeRotorGetPosition(struct Motor *motor);
