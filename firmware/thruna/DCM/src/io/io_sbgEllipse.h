#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "app_sbgEllipse.h"

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"
#endif

// Enum of all the values read from the SBG Ellipse N sensortypedef enum
typedef enum
{
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

    // Position Info
    ELLIPSE_OUTPUT_GPS_POS_STATUS,
    ELLIPSE_OUTPUT_GPS_LAT,
    ELLIPSE_OUTPUT_GPS_LAT_ACC,
    ELLIPSE_OUTPUT_GPS_LONG,
    ELLIPSE_OUTPUT_GPS_LONG_ACC,
    ELLIPSE_OUTPUT_GPS_ALT,
    ELLIPSE_OUTPUT_GPS_ALT_ACC,

    // Velocity Info
    ELLIPSE_OUTPUT_GPS_VEL_STATUS,
    ELLIPSE_OUTPUT_GPS_VEL_N,
    ELLIPSE_OUTPUT_GPS_VEL_N_ACC,
    ELLIPSE_OUTPUT_GPS_VEL_E,
    ELLIPSE_OUTPUT_GPS_VEL_E_ACC,
    ELLIPSE_OUTPUT_GPS_VEL_D,
    ELLIPSE_OUTPUT_GPS_VEL_D_ACC,

    NUM_SBG_OUTPUTS,
} SbgEllipseOutput;

/*
 * Initialize the SBG Ellipse N sensor IO module.
 */
bool io_sbgEllipse_init(UART *imu_uart);

/*
 * Parse all logs which are currently residing in the UART RX buffer.
 */
void io_sbgEllipse_handleLogs(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t io_sbgEllipse_getTimestampUs(void);

/*
 * Get a last read output from the SBG Ellipse N sensor.
 * This is a single function to avoid faking a whole bunch of IO functions for each sensor output.
 * @param output Which sensor value to read
 * @return Last read sensor output, in the cooresponding units
 */
float io_sbgEllipse_getSensorOutput(SbgEllipseOutput output);

/*
 * Get general status from the sensor.
 * @return Bitmask of faults, 1 indiciates a fault
 */
uint16_t io_sbgEllipse_getGeneralStatus(void);

/*
 * Get communication status from the sensor.
 * @return Bitmask of faults, 0 indiciates a fault
 */
uint32_t io_sbgEllipse_getComStatus(void);

/*
 * Get queue overflow status
 * @return the overflow uint32_t
 */
uint32_t io_sbgEllipse_getOverflowCount(void);
