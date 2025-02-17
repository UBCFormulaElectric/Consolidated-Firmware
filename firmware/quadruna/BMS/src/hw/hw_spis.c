#include "hw_spi.h"

#include "ltc6813/io_ltc6813Shared.h"

SPI_HandleTypeDef *const spi_bus_handles[HW_SPI_BUS_COUNT] = {
    [HW_SPI_BUS_2] = &hspi2,
};

const SpiDevice ltc6813_spi = { .spi_bus    = HW_SPI_BUS_2,
                                .nss_pin    = { .port = SPI_CS_GPIO_Port, .pin = SPI_CS_Pin },
                                .timeout_ms = LTC6813_SPI_TIMEOUT_MS };
