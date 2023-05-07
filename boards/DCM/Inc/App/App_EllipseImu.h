#pragma once

#include <stdint.h>

// Enum of all the values read from the SBG Ellipse N sensor
typedef enum {
  // Transational acceleration (m/s^2)
  ELLIPSE_OUTPUT_ACCELERATION_X,
  ELLIPSE_OUTPUT_ACCELERATION_Y,
  ELLIPSE_OUTPUT_ACCELERATION_Z,

  // Angular velocity (deg/s)
  ELLIPSE_OUTPUT_ANGULAR_VELOCITY_ROLL,
  ELLIPSE_OUTPUT_ANGULAR_VELOCITY_PITCH,
  ELLIPSE_OUTPUT_ANGULAR_VELOCITY_YAW,

  // Euler angles (deg)
  ELLIPSE_OUTPUT_EULER_ROLL,
  ELLIPSE_OUTPUT_EULER_PITCH,
  ELLIPSE_OUTPUT_EULER_YAW,

  NUM_SBG_OUTPUTS,
} EllipseImuOutput;

/*
 * Initialize the SBG Ellipse N sensor app module.
 */
void App_EllipseImu_Init(void (*handle_logs)(void),
                         uint32_t (*get_timestamp)(void),
                         uint16_t (*get_general_status)(void),
                         uint32_t (*get_com_status)(void),
                         float (*get_sensor_output)(EllipseImuOutput));

/*
 * Parse and save all received logs.
 */
void App_EllipseImu_HandleLogs(void);

/*
 * Broadcast sensor outputs over CAN.
 */
void App_EllipseImu_Broadcast(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t App_EllipseImu_GetTimestampUs();

/*
 * Get an output from the SBG Ellipse N sensor.
 * @param output Which sensor value to read
 * @return Last read sensor output, in the cooresponding units
 */
float App_EllipseImu_GetSensorOutput(EllipseImuOutput output);
