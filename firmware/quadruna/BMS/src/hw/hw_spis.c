#include "hw_spis.h"
#include "main.h"
#include "ltc6813/io_ltc6813Shared.h"

static SpiBus isospi_bus = { .handle = &hspi2 };

const SpiDevice ltc6813_spi = { .bus        = &isospi_bus,
                                .nss_pin    = { .port = SPI_CS_GPIO_Port, .pin = SPI_CS_Pin },
                                .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi2)
    {
        return &isospi_bus;
    }
    else
    {
        return NULL;
    }
}
