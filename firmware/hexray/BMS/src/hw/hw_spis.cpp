#include "hw_spis.hpp"
#include "hw_gpios.hpp"
#include "main.h"


#include <assert.h>

namespace hw::spi
{
static constexpr uint16_t ADBMS_SPI_TIMEOUT_MS = 100;

static SpiBus isospi_bus (hspi4 );

const SpiDevice adbms_spi_ls(isospi_bus, hw::gpio::spi_cs_ls,  ADBMS_SPI_TIMEOUT_MS);

const SpiDevice adbms_spi_hs(isospi_bus, hw::gpio::spi_cs_hs, ADBMS_SPI_TIMEOUT_MS);
} // namespace hw::spi
