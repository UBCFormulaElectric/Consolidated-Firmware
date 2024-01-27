#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"
#endif

// Enum of all the values read from the SBG Ellipse N sensor
typedef enum
{
    // Transational acceleration (m/s^2)
    SBG_ELLIPSE_OUT_ACCELERATION_X,
    SBG_ELLIPSE_OUT_ACCELERATION_Y,
    SBG_ELLIPSE_OUT_ACCELERATION_Z,

    // Angular velocity (deg/s)
    SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_ROLL,
    SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_PITCH,
    SBG_ELLIPSE_OUT_ANGULAR_VELOCITY_YAW,

    // Euler angles (deg)
    SBG_ELLIPSE_OUT_EULER_ROLL,
    SBG_ELLIPSE_OUT_EULER_PITCH,
    SBG_ELLIPSE_OUT_EULER_YAW,

    NUM_SBG_OUTPUTS,
} SbgEllipseOutput;

#ifdef TARGET_EMBEDDED
/*
 * Initialize the SBG Ellipse N sensor IO module.
 */
bool io_sbgEllipse_init(UART *imu_uart);
#endif

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
