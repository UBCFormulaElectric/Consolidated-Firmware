#include "hw_spis.hpp"
#include <cassert>
#include <hw_gpio.hpp>
#include <hw_spi.hpp>

#define TIMEOUT (100U)
// IMU 1
#define MASTER_NSS_Pin GPIO_PIN_4
#define MASTER_NSS_GPIO_Port GPIOA
// IMU 2
#define MASTER_NSS_Pin GPIO_PIN_4
#define MASTER_NSS_GPIO_Port GPIOA
// IMU 3
#define MASTER_NSS_Pin GPIO_PIN_4
#define MASTER_NSS_GPIO_Port GPIOA

namespace vc::hw::spis
{
const hw::Gpio IMU1();
IMU1.master_nss(MASTER_NSS_GPIO_Port, MASTER_NSS_Pin);
static hw::spi::SpiBus master_bus(hspi1);
hw::spi::SpiDevice     spi_device_master(master_bus, master_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle1)
{
    assert(handle == &master_bus.handle);
    return &master_bus;
}

const hw::Gpio IMU2();
IMU2.master_nss(MASTER_NSS_GPIO_Port, MASTER_NSS_Pin);
static hw::spi::SpiBus master_bus(hspi2);
hw::spi::SpiDevice     spi_device_master(master_bus, master_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle2)
{
    assert(handle == &master_bus.handle);
    return &master_bus;
}


const hw::Gpio IMU3();
IMU3.master_nss(MASTER_NSS_GPIO_Port, MASTER_NSS_Pin);
static hw::spi::SpiBus master_bus(hspi3);
hw::spi::SpiDevice     spi_device_master(master_bus, master_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle3)
{
    assert(handle == &master_bus.handle);
    return &master_bus;
}

} // namespace vc::hw::spis
