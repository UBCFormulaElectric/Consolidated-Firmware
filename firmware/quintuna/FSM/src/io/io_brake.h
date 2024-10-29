#pragma once
#include <cstdint>

#ifndef IO_BRAKE_H
#define IO_BRAKE_H


namespace io::brake
{
bool isActuated();
float io_brake_getFrontPressurePsi();
bool io_brake_frontPressureSensorOCSC();
float io_brake_getRearPressurePsi();
bool io_brake_rearPressureSensorOCSC();
bool io_brake_hwOCSC();
}

#endif //IO_BRAKE_H
