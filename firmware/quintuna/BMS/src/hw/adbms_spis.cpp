#include <stdint.h>

extern "C"
{
#include "hw_spis.h"
}

static constexpr uint32_t ADBMS_SPI_TIMEOUT_MS = 100U;

extern const SpiDevice adbms_spi_ls = { &isospi_bus, &spi_cs_ls_pin, ADBMS_SPI_TIMEOUT_MS };
extern const SpiDevice adbms_spi_hs = { &isospi_bus, &spi_cs_hs_pin, ADBMS_SPI_TIMEOUT_MS };
