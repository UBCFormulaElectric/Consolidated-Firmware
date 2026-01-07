#pragma once

extern "C"
{
    #include "hw_spi.h"
}

namespace hw::spi
{
    extern SpiBus spi3;
    extern SpiDevice imu;
}

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle);