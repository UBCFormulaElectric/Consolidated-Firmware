#include "hw_spis.hpp"
#include <cassert>
#include <hw_gpio.hpp>
#include <hw_spi.hpp>

#define TIMEOUT (100U)

const hw::Gpio         master_nss(MASTER_NSS_GPIO_Port, MASTER_NSS_Pin);
static hw::spi::SpiBus master_bus(hspi1);
hw::spi::SpiDevice     spi_device_master(master_bus, master_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &master_bus.handle);
    return &master_bus;
}
