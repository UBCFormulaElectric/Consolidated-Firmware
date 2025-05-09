#include "io_potentiometer.h"
#include "hw_utils.h"
#include "hw_i2cs.h"
#include <stdbool.h>

#define MAX_WIPER_VALUE (256)

typedef enum
{
    POTENTIOMETER_WRITE_CMD = 0x00,
    POTENTIOMETER_INC_CMD   = 0x01,
    POTENTIOMETER_DEC_CMD   = 0x02,
    POTENTIOMETER_READ_CMD  = 0x03,
} POTENTIOMETER_CMD;

ExitCode io_potentiometer_readPercentage(const Potentiometer *potentiometer, POTENTIOMETER_ADD wiper, uint8_t *dest)
{
    uint8_t data;
    RETURN_IF_ERR(io_potentiometer_readWiper(potentiometer->i2c_handle, wiper, &data));
    *dest = (uint8_t)((uint16_t)data * 100 / MAX_WIPER_VALUE);
    return EXIT_CODE_OK;
}

ExitCode
    io_potentiometer_writePercentage(const Potentiometer *potentiometer, POTENTIOMETER_ADD wiper, uint8_t percentage)
{
    return io_potentiometer_writeWiper(
        potentiometer->i2c_handle, wiper, CLAMP(percentage, 0, 100) * MAX_WIPER_VALUE / 100);
}

ExitCode io_potentiometer_readWiper(const Potentiometer *potentiometer, POTENTIOMETER_ADD wiper, uint8_t *dest)
{
    return hw_i2c_memoryRead(potentiometer->i2c_handle, wiper, dest, sizeof(*dest));
}

ExitCode io_potentiometer_writeWiper(const Potentiometer *potentiometer, POTENTIOMETER_ADD wiper, uint8_t data)
{
    uint8_t packet[2] = { (uint8_t)((uint8_t)wiper | (uint8_t)POTENTIOMETER_WRITE_CMD), data };
    return hw_i2c_transmit(potentiometer->i2c_handle, packet, sizeof(packet));
}