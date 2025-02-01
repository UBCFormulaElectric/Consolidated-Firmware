#include "hw_i2c.h"
#include <assert.h>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "main.h"

typedef struct
{
    StaticSemaphore_t sem_ctrl_block;
    osSemaphoreId_t   sem_id;
} I2cBusData;

typedef struct
{
    I2C_HandleTypeDef *handle;
    osSemaphoreAttr_t  sem_attr;
} I2cBusCfg;

static I2cBusData bus_data[HW_I2C_BUS_COUNT];

static const I2cBusCfg bus_cfg[HW_I2C_BUS_COUNT] = {
    [HW_I2C_BUS_1] = { .handle   = &hi2c1,
                       .sem_attr = { .name      = "I2C Bus 1 Semaphore",
                                     .attr_bits = 0,
                                     .cb_mem    = &bus_data[HW_I2C_BUS_1].sem_ctrl_block,
                                     .cb_size   = sizeof(StaticSemaphore_t) } },
    [HW_I2C_BUS_2] = { .handle   = &hi2c2,
                       .sem_attr = { .name      = "I2C Bus 2 Semaphore",
                                     .attr_bits = 0,
                                     .cb_mem    = &bus_data[HW_I2C_BUS_2].sem_ctrl_block,
                                     .cb_size   = sizeof(StaticSemaphore_t) } },
};

// Number of attempts made to check if connected device is ready to communicate.
#define NUM_DEVICE_READY_TRIALS 5

static inline bool handleToI2cBus(I2C_HandleTypeDef *const handle, I2cBus *bus)
{
    for (I2cBus i = 0; i < HW_I2C_BUS_COUNT; i++)
    {
        if (bus_cfg[i].handle == handle)
        {
            *bus = (I2cBus)i;
            return true;
        }
    }

    return false;
}

void hw_i2c_init(void)
{
    for (I2cBus i = 0U; i < HW_I2C_BUS_COUNT; i++)
    {
        bus_data[i].sem_id = osSemaphoreCreate(&bus_cfg[i].sem_attr, 1);

        // Default all semaphores to "acquired."
        assert(osSemaphoreAcquire(bus_data[i].sem_id, 0U) == osOK);
    }
}

bool hw_i2c_isTargetReady(const I2cDevice *device)
{
    return HAL_I2C_IsDeviceReady(
               bus_cfg[device->bus].handle, device->target_address << 1, (uint32_t)NUM_DEVICE_READY_TRIALS,
               device->timeout_ms) == HAL_OK;
}

bool hw_i2c_transmit(const I2cDevice *device, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (HAL_I2C_Master_Transmit(
            bus_cfg[device->bus].handle, device->target_address << 1, tx_buffer, tx_buffer_size, device->timeout_ms) !=
        HAL_OK)
    {
        return false;
    }

    // Block until semaphore is released by ISR.
    osSemaphoreAcquire(bus_data[device->bus].sem_id, osWaitForever);

    return true;
}

bool hw_i2c_receive(const I2cDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (HAL_I2C_Master_Receive(
            bus_cfg[device->bus].handle, device->target_address << 1, rx_buffer, rx_buffer_size, device->timeout_ms) !=
        HAL_OK)
    {
        return false;
    }

    // Block until semaphore is released by ISR.
    osSemaphoreAcquire(bus_data[device->bus].sem_id, osWaitForever);

    return true;
}

bool hw_i2c_memRead(const I2cDevice *device, uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (HAL_I2C_Mem_Read(
            bus_cfg[device->bus].handle, device->target_address << 1, mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer,
            rx_buffer_size, device->timeout_ms) != HAL_OK)
    {
        return false;
    }

    // Block until semaphore is released by ISR.
    osSemaphoreAcquire(bus_data[device->bus].sem_id, osWaitForever);

    return true;
}

bool hw_i2c_memWrite(const I2cDevice *device, uint16_t mem_addr, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (HAL_I2C_Mem_Write(
            bus_cfg[device->bus].handle, device->target_address << 1, mem_addr, I2C_MEMADD_SIZE_8BIT, tx_buffer,
            tx_buffer_size, device->timeout_ms) != HAL_OK)
    {
        return false;
    }

    // Block until semaphore is released by ISR.
    osSemaphoreAcquire(bus_data[device->bus].sem_id, osWaitForever);

    return true;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *handle)
{
    I2cBus bus;
    assert(handleToI2cBus(handle, &bus));
    osSemaphoreRelease(bus_data[bus].sem_id);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *handle)
{
    I2cBus bus;
    assert(handleToI2cBus(handle, &bus));
    osSemaphoreRelease(bus_data[bus].sem_id);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *handle)
{
    I2cBus bus;
    assert(handleToI2cBus(handle, &bus));
    osSemaphoreRelease(bus_data[bus].sem_id);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *handle)
{
    I2cBus bus;
    assert(handleToI2cBus(handle, &bus));
    osSemaphoreRelease(bus_data[bus].sem_id);
}
