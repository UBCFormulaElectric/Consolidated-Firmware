#include "io_shift_register.h"
#include "hw_spi.h"

bool io_shift_register_writeBytes(const SpiDevice *spi, const uint8_t *buffer, uint16_t length)
{
    return hw_spi_transmit(spi, (uint8_t *)buffer, length);
}
