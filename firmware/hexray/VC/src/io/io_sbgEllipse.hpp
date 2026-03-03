#pragma once 

#include <expected>
#include <cstdint>

#include "util_errorCodes.hpp"

namespace io::sbgEllipse {

class Vector3 { 
    public: 
        float x;
        float y;
        float z;
};

class Attitude {
    public: 
        float roll;
        float pitch;
        float yaw;
};

class PositionData {
    public: 
        uint32_t status;
        double   latitude;
        double   longitude;
        double   altitude;
        float    latitude_std_dev;
        float    longitude_std_dev;
        float    altitude_std_dev;
};

class VelocityData {
    public:
        uint32_t status;
        float    north;
        float    east;
        float    down;
        float    north_std_dev;
        float    east_std_dev;
        float    down_std_dev;
};

class EkfNavPacketData {
    public:
        VelocityData velocity;
        PositionData position;
};

class ImuPacketData {
    public: 
        Vector3  acceleration;
        Attitude angular_velocity;
};

class EkfEulerPacketData {
    public:
        Attitude euler_angles;
};

class StatusPacketData {
    public:
        uint32_t timestamp_us;
        uint16_t general_status;
        uint32_t com_status;
};

class SensorData
{
    public:
        ImuPacketData      imu_data;
        EkfEulerPacketData ekf_euler_data;
        StatusPacketData   status_data;
        EkfNavPacketData   ekf_nav_data;
        uint32_t           ekf_solution_status;
};

std::expected<void, ErrorCode> init();

/*
 * Parse all logs which are currently residing in the UART RX buffer.
 */
void handleLogs(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t getTimestampUs(void);

/*
 * Get general status from the sensor.
 * @return Bitmask of faults, 1 indiciates a fault
 */
uint16_t getGeneralStatus(void);

/*
 * Get communication status from the sensor.
 * @return Bitmask of faults, 0 indiciates a fault
 */
uint32_t getComStatus(void);

/*
 * Get queue overflow status
 * @return the overflow uint32_t
 */
uint32_t getOverflowCount(void);

/*
 * Get EKF Solution Mode Status
 * @return the ekf solution mode
 */
uint32_t getEkfSolutionMode(void);

/**
 * Get the IMU accelerations as a struct pointer with fields:
 * - float x: Forward acceleration in m/s^2
 * - float y: Lateral acceleration in m/s^2
 * - float z: Vertical acceleration in m/s^2
 */
Vector3 *getImuAccelerations(void);

/*
 * Get the IMU angular velocities as a struct pointer with fields:
 * - float roll: Roll angular velocity in rad/s
 * - float pitch: Pitch angular velocity in rad/s
 * - float yaw: Yaw angular velocity in rad/s
 */
Attitude *getImuAngularVelocities(void);

/*
 * Get the Euler angles as a struct pointer with fields:
 * - float roll: Roll angle in rad
 * - float pitch: Pitch angle in rad
 * - float yaw: Yaw angle in rad
 */
Attitude *getEkfEulerAngles(void);

/*
 * Get the GPS velocity data as a struct pointer with fields:
 * - SbgEComGpsVelStatus status: Status of the velocity data
 * - float velocity_n: North velocity in m/s
 * - float velocity_e: East velocity in m/s
 * - float velocity_d: Down velocity in m/s
 * - float velocity_accuracy_n: North velocity accuracy in m/s
 * - float velocity_accuracy_e: East velocity accuracy in m/s
 * - float velocity_accuracy_d: Down velocity accuracy in m/s
 */
VelocityData *getEkfNavVelocityData(void);

/*
 * Get the GPS position data as a struct pointer with fields:
 * - SbgEComGpsPosStatus status: Status of the position data
 * - double latitude: Latitude in degrees
 * - double longitude: Longitude in degrees
 * - double altitude: Altitude in meters
 * - float latitude_accuracy: Latitude accuracy in meters
 * - float longitude_accuracy: Longitude accuracy in meters
 * - float altitude_accuracy: Altitude accuracy in meters
 *
 */
PositionData *getEkfNavPositionData(void);

/*
 * Handle SBG Ellipse UART Callbacks
 */
void msgRxCallback(void);

} //namespace io
