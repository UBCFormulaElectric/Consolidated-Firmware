#include "Io_BrakeLight.h"
#include "Io_Buzzer.h"
#include "Io_Imu.h"
#include "Io_InverterSwitches.h"
#include <string.h>

typedef struct
{
    // IMU data
    float accel_x;
    float accel_y;
    float accel_z;

    // Inverter switches data
    bool right_inverter_switch_on;
    bool left_inverter_switch_on;

} MockDcmIo_Data;

MockDcmIo_Data io;

void Io_MockDcmIo_Init()
{
    memset(&io, 0, sizeof(MockDcmIo_Data));
}

/* ----------------------- Mock Setters ------------------------*/

void Test_MockDcmIo_SetAccelX(float accel_x)
{
    io.accel_x = accel_x;
}

void Test_MockDcmIo_SetAccelY(float accel_y)
{
    io.accel_y = accel_y;
}

void Test_MockDcmIo_SetAccelZ(float accel_z)
{
    io.accel_z = accel_z;
}

/* ----------------------- Brake lights ------------------------*/

void Io_BrakeLight_TurnOn(void) {}
void Io_BrakeLight_TurnOff(void) {}

/* -------------------------- Buzzer ---------------------------*/

void Io_Buzzer_TurnOn(void) {}
void Io_Buzzer_TurnOff(void) {}

/* --------------------------- IMU -----------------------------*/

float Io_Imu_GetAccelerationX()
{
    return io.accel_x;
}

float Io_Imu_GetAccelerationY()
{
    return io.accel_y;
}

float Io_Imu_GetAccelerationZ()
{
    return io.accel_z;
}

/* --------------------- Inverter switches------------------------*/

void Io_InverterSwitches_TurnOnLeft()
{
    io.left_inverter_switch_on = true;
}

void Io_InverterSwitches_TurnOffLeft()
{
    io.left_inverter_switch_on = false;
}

void Io_InverterSwitches_TurnOnRight()
{
    io.right_inverter_switch_on = true;
}

void Io_InverterSwitches_TurnOffRight()
{
    io.right_inverter_switch_on = false;
}

bool Io_InverterSwitches_IsRightInverterOn()
{
    return io.right_inverter_switch_on;
}

bool Io_InverterSwitches_IsLeftInverterOn()
{
    return io.left_inverter_switch_on;
}
