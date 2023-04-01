#pragma once

#include <stdint.h>

// Enum of all the values read from the SBG Ellipse N sensor
typedef enum
{
    // Transational acceleration (m/s^2)
    SBG_OUTPUT_ACCELERATION_X,
    SBG_OUTPUT_ACCELERATION_Y,
    SBG_OUTPUT_ACCELERATION_Z,

    // Angular velocity (deg/s)
    SBG_OUTPUT_ANGULAR_VELOCITY_ROLL,
    SBG_OUTPUT_ANGULAR_VELOCITY_PITCH,
    SBG_OUTPUT_ANGULAR_VELOCITY_YAW,

    // Euler angles (deg)
    SBG_OUTPUT_EULER_ROLL,
    SBG_OUTPUT_EULER_PITCH,
    SBG_OUTPUT_EULER_YAW,

    NUM_SBG_OUTPUTS,
} SbgSensorOutput;

/*
 * Initialize the SBG Ellipse N sensor app module.
 */
void App_SbgEllipseN_Init(
    void (*handle_logs)(void),
    uint32_t (*get_timestamp)(void),
    uint16_t (*get_general_status)(void),
    uint32_t (*get_com_status)(void),
    float (*get_sensor_output)(SbgSensorOutput));

/*
 * Parse and save all received logs.
 */
void App_SbgEllipseN_HandleLogs(void);

/*
 * Broadcast sensor outputs over CAN.
 */
void App_SbgEllipseN_Broadcast(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t App_SbgEllipseN_GetTimestampUs();

/*
 * Get an output from the SBG Ellipse N sensor.
 * @param output Which sensor value to read
 * @return Last read sensor output, in the cooresponding units
 */
float App_SbgEllipseN_GetSensorOutput(SbgSensorOutput output);
