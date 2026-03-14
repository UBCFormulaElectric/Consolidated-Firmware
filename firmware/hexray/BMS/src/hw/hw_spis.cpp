#include "hw_spis.hpp"
#include "hw_gpios.hpp"
#include "main.h"

#include <cassert>

static constexpr uint16_t ADBMS_SPI_TIMEOUT_MS = 100;

static hw::spi::SpiBus   isospi_bus(hspi4);
const hw::spi::SpiDevice adbms_spi_ls(isospi_bus, spi_cs_ls, ADBMS_SPI_TIMEOUT_MS);
const hw::spi::SpiDevice adbms_spi_hs(isospi_bus, spi_cs_hs, ADBMS_SPI_TIMEOUT_MS);

const SpiDevice adbms_spi_ls = { .bus = &isospi_bus, .nss_pin = &spi_cs_ls_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };

const SpiDevice adbms_spi_hs = { .bus = &isospi_bus, .nss_pin = &spi_cs_hs_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };
} // namespace hw::spi

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == isospi_bus.handle);
    return &isospi_bus;
}