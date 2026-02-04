#include "hw_spis.hpp"

#include <assert.h>

static constexpr uint16_t ADBMS_SPI_TIMEOUT_MS = 100;

static SpiBus isospi_bus = { .handle = &hspi4, .task_in_progress = NULL };

const SpiDevice adbms_spi_ls = { .bus = &isospi_bus, .nss_pin = &spi_cs_ls_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };

const SpiDevice adbms_spi_hs = { .bus = &isospi_bus, .nss_pin = &spi_cs_hs_pin, .timeout_ms = ADBMS_SPI_TIMEOUT_MS };

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == isospi_bus.handle);
    return &isospi_bus;
}