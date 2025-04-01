#include "io_dac.h"

#include "app_utils.h"
#include "hw_spis.h"
#include "hw_gpios.h"

#include <assert.h>

#define V_REF (4.576f)
#define RESOLUTION (12)

bool io_dac_writeReg(const DACChannel reg_addr, const float volts)
{
    const uint16_t data = (uint16_t)(volts * (1 << RESOLUTION) / V_REF) & 0xFFF;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    struct __attribute__((packed))
    {
        // byte 1
        uint8_t address : 4;
        uint8_t cmd : 4;

        // byte 2
        uint8_t data_1;

        // byte 3
        uint8_t data_2;
    } tx_msg = { .cmd = 0x3, .address = reg_addr, .data_1 = 0xff & data >> 4, .data_2 = 0xf & data };
    static_assert(sizeof(tx_msg) == 3);
#pragma GCC diagnostic pop

    assert(reg_addr == tx_msg.address);
    return hw_spi_transmit(&dac, (uint8_t *)&tx_msg, sizeof(tx_msg));
}

bool io_dac_powerUp(const DACChannel reg_addr)
{
    struct __attribute__((packed))
    {
        uint8_t cmd : 4;
        uint8_t address : 4;
    } tx_msg = { .cmd = 0x1, .address = reg_addr };
    assert(reg_addr == tx_msg.address);
    return hw_spi_transmit(&dac, (uint8_t *)&tx_msg, sizeof(tx_msg));
}

bool io_dac_powerDown(const DACChannel reg_addr)
{
    struct __attribute__((packed))
    {
        uint8_t cmd : 4;
        uint8_t address : 4;
    } tx_msg = { .cmd = 0x4, .address = reg_addr };
    assert(reg_addr == tx_msg.address);
    return hw_spi_transmit(&dac, (uint8_t *)&tx_msg, sizeof(tx_msg));
}

void io_dac_clear()
{
    hw_gpio_writePin(&dac_n_clear, false);
    HAL_Delay(1);
    hw_gpio_writePin(&dac_n_clear, true);
}