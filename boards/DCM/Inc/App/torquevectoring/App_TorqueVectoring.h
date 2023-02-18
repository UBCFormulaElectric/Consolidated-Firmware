//
// Created by formulae on 18/02/23.
//

#ifndef FORMULA_ELECTRIC_FIRMWARE_APP_TORQUEVECTORING_H
#define FORMULA_ELECTRIC_FIRMWARE_APP_TORQUEVECTORING_H

typedef struct PowerLimiting_Inputs {
    float left_motor_temp_celsius;
    float right_motor_temp_celsius;
    float available_battery_power_kW;
    float accelerator_pedal_percentage;
}PowerLimiting_Inputs;
typedef struct ActiveDifferential_Inputs {
    float steering_angle_degrees;
    float left_wheel_speed_rpm;
    float right_wheel_speed_rpm;
    float max_power_kW;
}ActiveDifferential_Inputs;

#endif //FORMULA_ELECTRIC_FIRMWARE_APP_TORQUEVECTORING_H
