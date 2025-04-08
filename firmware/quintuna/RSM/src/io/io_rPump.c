#include "hw_i2cs.h"

// SOURCE:
// https://datasheet.ciiva.com/41459/001083254-da-01-en-ic-dgtl-pot-mcp4661-502e-st-tssop-14-mcp-41459017.pdf?src-supplier=Conrad
#define MAX_WIPER_VALUE (256.0f)
#define MIN_WIPER_VALUE (0.0f)
#define RPUMP_WIPER_REGISTER (0x02)

bool io_rPump_isPumpReady()
{
    return hw_i2c_isTargetReady(&r_pump_i2c);
}

bool io_rPump_write(uint8_t input)
{
    return hw_i2c_memoryWrite(&r_pump_i2c, RPUMP_WIPER_REGISTER, &input, sizeof(input));
}

bool io_rPump_read(uint8_t *dest)
{
    return hw_i2c_memoryRead(&r_pump_i2c, RPUMP_WIPER_REGISTER, dest, sizeof(dest));
}

bool io_rPump_setPercentage(float percentage)
{
    if (percentage > 100.0f)
    {
        percentage = 100.0f;
    }
    else if (percentage < 0.0f)
    {
        percentage = 0.0f;
    }
    uint8_t data = (uint8_t)((1.0f - (percentage / 100.0f)) * MAX_WIPER_VALUE);
    return io_rPump_write(data);
}

bool io_rPump_readPercentage(float *dest)
{
    uint8_t data;
    if (!io_rPump_read(&data))
    {
        return false;
    }
    *dest = (1.0f - ((float)data / MAX_WIPER_VALUE)) * 100.0f;
    return true;
}