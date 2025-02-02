#pragma once

void app_torqueVectoring_init(void);
void app_torqueVectoring_run(float accelerator_pedal_percentage);
float app_torqueVectoring_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);
