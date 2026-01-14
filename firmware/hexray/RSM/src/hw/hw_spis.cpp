#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

namespace hw::spi
{
// why isnt IMU_CS_Pin an address and instead its a uint_16
// determine timeout_ms w/ datasheet of imu?

SpiBus    spi3(hspi3);
SpiDevice imu(spi3, gpio::imu_cs, 10);

// should I write if (handle != &hspi) {assert(false); return nullptr} else {return &spi3} instead?
// or does this count as hard coding?
SpiBus &hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi3);
    return spi3;
    // should I add a while (1) to prevent other execution to emphasize hspi3 is not handle
}

} // namespace hw::spi