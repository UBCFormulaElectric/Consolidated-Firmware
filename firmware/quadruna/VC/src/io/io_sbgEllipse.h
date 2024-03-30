#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "sbgECom.h"

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"
#endif

/* ------------------------------------ Typedefs ------------------------------------- */

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} Attitude;

typedef struct
{
    SbgEComGpsPosStatus status;
    double              latitude;
    double              longitude;
    double              altitude;
    float               latitude_accuracy;
    float               longitude_accuracy;
    float               altitude_accuracy;
} GpsPositionData;

typedef struct
{
    SbgEComGpsVelStatus status;
    float               velocity_n; // North
    float               velocity_e; // East
    float               velocity_d; // Down
    float               velocity_accuracy_n;
    float               velocity_accuracy_e;
    float               velocity_accuracy_d;
} GpsVelocityData;

typedef struct
{
    GpsVelocityData gps1_velocity;
    GpsPositionData gps1_position;
} Gps1Data;

typedef struct
{
    Vector3  acceleration;
    Attitude angular_velocity;
} ImuPacketData;

typedef struct
{
    Attitude euler_angles;
} EulerPacketData;

typedef struct
{
    uint32_t timestamp_us;
    uint16_t general_status;
    uint32_t com_status;
} StatusPacketData;

typedef struct
{
    ImuPacketData    imu_data;
    EulerPacketData  euler_data;
    StatusPacketData status_data;
    Gps1Data         gps_data;
} SensorData;

#ifdef TARGET_EMBEDDED
#include "hw_uart.h"
/*
 * Initialize the SBG Ellipse N sensor IO module.
 */
bool io_sbgEllipse_init(const UART *imu_uart);
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
float io_sbgEllipse_getSensorOutput();

SensorData *io_sbgEllipse_getSensorData();

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

// TODO: Write function descriptions
Vector3         *io_sbgEllipse_getImuAccelerations();
Attitude        *io_sbgEllipse_getImuAngularVelocities();
Attitude        *io_sbgEllipse_getEulerAngles();
GpsVelocityData *io_sbgEllipse_getGpsVelocityData();
GpsPositionData *io_sbgEllipse_getGpsPositionData();