#include "main.h"
#include "hw_spis.hpp"
#include "hw_gpio.hpp"

#include <cassert>

static constexpr uint32_t ADBMS_SPI_TIMEOUT_MS = 100U;

const hw::Gpio spi_cs_ls_pin{ SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin };
const hw::Gpio spi_cs_hs_pin{ SPI_CS_HS_GPIO_Port, SPI_CS_HS_Pin };

namespace hw::spi
{
constexpr SpiBus    isospi_bus{ hspi4 };
constexpr SpiDevice adbms_spi_ls{ isospi_bus, spi_cs_ls_pin, ADBMS_SPI_TIMEOUT_MS };
constexpr SpiDevice adbms_spi_hs{ isospi_bus, spi_cs_hs_pin, ADBMS_SPI_TIMEOUT_MS };

[[nodiscard]] const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi4);
    return isospi_bus;
}
} // namespace hw::spi