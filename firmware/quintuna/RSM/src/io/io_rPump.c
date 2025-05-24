#include "hw_i2cs.h"
#include "io_rPump.h"
#include "io_potentiometer.h"

// SOURCE (for pot in shared file):
// https://datasheet.ciiva.com/41459/001083254-da-01-en-ic-dgtl-pot-mcp4661-502e-st-tssop-14-mcp-41459017.pdf?src-supplier=Conrad

static const Potentiometer rsm_pot = {
    .i2c_handle = &r_pump_i2c,
};

#define RPUMP_WIPER (0x00)
#define MAX_WIPER_VALUE (256.0f)

ExitCode io_rPump_isPumpReady()
{
    return hw_i2c_isTargetReady(&r_pump_i2c);
}

ExitCode io_rPump_write(const uint8_t data)
{
    return io_potentiometer_writeWiper(&rsm_pot, RPUMP_WIPER, data);
}

ExitCode io_rPump_read(uint8_t *dest)
{
    return io_potentiometer_readWiper(&rsm_pot, RPUMP_WIPER, dest);
}

ExitCode io_rPump_setPercentage(float percentage)
{
    percentage = CLAMP(percentage, 0.0f, 100.0f);
    return io_rPump_write((uint8_t)((1.0f - percentage / 100.0f) * MAX_WIPER_VALUE));
}

ExitCode io_rPump_readPercentage(float *dest)
{
    uint8_t data;
    RETURN_IF_ERR(io_potentiometer_readPercentage(&rsm_pot, RPUMP_WIPER, &data));
    *dest = (1.0f - (float)data / MAX_WIPER_VALUE) * 100.0f;
    return true;
}