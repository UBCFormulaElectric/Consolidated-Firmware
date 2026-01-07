#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"
extern "C"
{
    #include "hw_spi.h"
}

//should hw_spi.h be refactored to c++?
namespace hw::spi{
    //hspi3 not linking properly
    //what is task_in_progress?
    SpiBus spi3 = {.handle = &hspi3, .task_in_progress = NULL};
    //why isnt IMU_CS_Pin an address and instead its a uint_16
    //determine timeout_ms w/ datasheet of imu?
    SpiDevice imu = {.bus = &spi3, .nss_pin = &imu_cs, .timeout_ms = 10};

    //should I do &hw_spi_get... instead of *hw_spi_get...
    //should I write if (handle != &hspi) {assert(false); return nullptr} else {return &spi3} instead?
    //or does this count as hard coding?
    SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
    {
        assert(handle == &hspi3);
        return &spi3; 
    }

}