#pragma once
#include "hw_spi.hpp"

namespace hw::spi
{
extern SpiBus    spi3;
extern SpiDevice imu;
SpiBus          &hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle);
} // namespace hw::spi
