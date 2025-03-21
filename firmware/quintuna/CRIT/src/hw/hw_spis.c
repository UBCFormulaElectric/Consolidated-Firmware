#include "hw_spis.h"
#include "hw_gpios.h"
#include "main.h"

static SpiBus led_spi_bus       = { .handle = &hspi2 };
static SpiBus seven_seg_spi_bus = { .handle = &hspi3 };

const SpiDevice led_spi       = { .bus = &led_spi_bus, .nss_pin = &led_serin, .timeout_ms = 10u };
const SpiDevice seven_seg_spi = { .bus = &seven_seg_spi_bus, .nss_pin = &seven_seg_serin, .timeout_ms = 10u };

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi2)
    {
        return &led_spi_bus;
    }
    else if (handle == &hspi3)
    {
        return &seven_seg_spi_bus;
    }
    else
    {
        return NULL;
    }
}
