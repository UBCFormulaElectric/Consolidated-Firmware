#pragma once

#include "hw_spi.h"

typedef enum
{
    HW_SPI_BUS_2,
    HW_SPI_BUS_COUNT
} SpiBus;

extern SPI_HandleTypeDef *const spi_bus_handles[HW_SPI_BUS_COUNT];
extern const SpiDevice          ltc6813_spi;
