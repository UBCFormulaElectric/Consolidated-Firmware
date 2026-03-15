#include "hw_spis.hpp"
#include "hw_gpios.hpp"
#include "main.h"

#include <cassert>

static constexpr uint32_t ADBMS_SPI_TIMEOUT_MS = 100;
namespace hw::spi
{

static SpiBus isospi_bus(hspi4);

const SpiDevice adbms_spi_ls(isospi_bus, spi_cs_ls, ADBMS_SPI_TIMEOUT_MS);
const SpiDevice adbms_spi_hs(isospi_bus, spi_cs_hs, ADBMS_SPI_TIMEOUT_MS);

const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &isospi_bus.getHandle());
    return isospi_bus;
}
} // namespace hw::spi
