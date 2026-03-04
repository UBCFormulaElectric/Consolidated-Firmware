#include "app_screens.hpp"
// #include <cstdint>
// #include "app_canTx.h"
// #include "app_canRx.h"

// typedef struct
// {
//     uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
//     uint8_t min_cell_voltage;
//     uint8_t power_draw;
//     uint8_t max_motor_temp;
// } app_screen_main_drive_data_t;

// /*********************** Function Definitions ***************************/
static void main_drive_update()
{
    //     // Get inputs.
    //     uint8_t fl_motor_temp = (uint8_t)app_canRx_INVFL_MotorTemperature_get();
    //     uint8_t fr_motor_temp = (uint8_t)app_canRx_INVFR_MotorTemperature_get();
    //     uint8_t rl_motor_temp = (uint8_t)app_canRx_INVRL_MotorTemperature_get();
    //     uint8_t rr_motor_temp = (uint8_t)app_canRx_INVRR_MotorTemperature_get();
    //
    //     uint8_t min_cell_voltage = (uint8_t)app_canRx_BMS_MinCellVoltage_get();
    //     uint8_t power_draw       = (uint8_t)app_canRx_BMS_TractiveSystemPower_get();
    //
    //     // Max motor temp
    //     uint8_t max_motor_temp = (uint8_t)fmax(fl_motor_temp, fr_motor_temp);
    //     max_motor_temp         = (uint8_t)fmax(max_motor_temp, rl_motor_temp);
    //     max_motor_temp         = (uint8_t)fmax(max_motor_temp, rr_motor_temp);
    //
    //     // Update min cell voltage data buffer.
    //     instance.data_buffer[0] = digit_to_segment(min_cell_voltage / 100) | SEG_PATTERN_DP;
    //     instance.data_buffer[1] = digit_to_segment((min_cell_voltage / 10) % 10);
    //     instance.data_buffer[2] = digit_to_segment(min_cell_voltage % 10);
    //
    //     // Update power draw data buffer.
    //     instance.data_buffer[3] = digit_to_segment(power_draw / 100);
    //     instance.data_buffer[4] = digit_to_segment((power_draw / 10) % 10);
    //     instance.data_buffer[5] = digit_to_segment(power_draw % 10);
    //
    //     // Update max motor temp data buffer.
    //     instance.data_buffer[6] = digit_to_segment(max_motor_temp / 100);
    //     instance.data_buffer[7] = digit_to_segment((max_motor_temp / 10) % 10);
    //     instance.data_buffer[8] = digit_to_segment(max_motor_temp % 10);
    //
    //     io_shift_register_updateSevenSegRegisters((uint8_t *)instance.data_buffer);
}

constexpr app::screens::Screen main_drive_screen = { .ccw_callback = nullptr,
                                                     .cw_callback  = nullptr,
                                                     .update       = main_drive_update };
