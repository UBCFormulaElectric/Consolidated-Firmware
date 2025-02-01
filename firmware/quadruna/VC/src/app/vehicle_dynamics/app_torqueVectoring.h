#pragma once

typedef enum{
    OFF, 
    TRACTION_CONTROL,
    ACTIVE_DIFF
}torqueVectoringStates;

void app_torqueVectoring_init(void);
void app_torqueVectoring_run(float accelerator_pedal_percentage);
void app_torqueVectoring_stateMachine(float pedal_percentage);
float app_torqueVectoring_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);
