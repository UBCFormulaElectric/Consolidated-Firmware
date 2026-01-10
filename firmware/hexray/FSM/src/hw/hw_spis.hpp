#pragma once
extern "C"
{
#include "main.h"
}
#include "hw_spi.hpp"

extern hw::spi::SpiDevice spi_device_master;
extern hw::spi::SpiDevice spi_device_slave;
