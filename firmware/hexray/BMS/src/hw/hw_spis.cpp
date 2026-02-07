#include "hw_spis.hpp"

#include <assert.h>

namespace hw::spi
{
static constexpr uint16_t ADBMS_SPI_TIMEOUT_MS = 100;

static SpiBus isospi_bus = { .handle = &hspi4 };

const SpiDevice adbms_spi_ls = { .bus = &isospi_bus, .nss_pin = &spi_cs_ls_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };

const SpiDevice adbms_spi_hs = { .bus = &isospi_bus, .nss_pin = &spi_cs_hs_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };
} // namespace hw::spi
