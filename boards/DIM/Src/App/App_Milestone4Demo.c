#include "../../TL85/scripts/TL85.h"
#include "App_Milestone4Demo.h"

static float prv_wheel_speed;
static float prv_battery_voltage;
static float prv_tire_pressure;

TL85 void SetWheelSpeed(float wheel_speed)
{
    prv_wheel_speed = wheel_speed;
}

TL85 void SetBatteryVoltage(float battery_voltage)
{
    prv_battery_voltage = battery_voltage;
}

TL85 void SetTirePressure(float tire_pressure)
{
    prv_tire_pressure = tire_pressure;
}
