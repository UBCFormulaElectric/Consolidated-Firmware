#include "hw_spis.hpp"
#include <cassert>
#include <hw_gpio.hpp>
#include <hw_spi.hpp>

#define TIMEOUT (1000U)

const hw::Gpio master_nss(MASTER_NSS_GPIO_Port, MASTER_NSS_Pin);
const hw::Gpio slave_nss(SLAVE_NSS_GPIO_Port, SLAVE_NSS_Pin);

static hw::spi::SpiBus master_bus(hspi1);
static hw::spi::SpiBus slave_bus(hspi2);

hw::spi::SpiDevice spi_device_master(master_bus, master_nss, TIMEOUT);
hw::spi::SpiDevice spi_device_slave(slave_bus, slave_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &master_bus.handle)
    {
        return &master_bus;
    }
    else if (handle == &slave_bus.handle)
    {
        return &slave_bus;
    }
    else
    {
        return nullptr;
    }
}
