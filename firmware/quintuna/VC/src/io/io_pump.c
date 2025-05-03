#include "app_utils.h"
#include "hw_i2cs.h"
#include "hw_utils.h"
#include <stdint.h>
#include "io_pump.h"

// SOURCE:
// https://datasheet.ciiva.com/41459/001083254-da-01-en-ic-dgtl-pot-mcp4661-502e-st-tssop-14-mcp-41459017.pdf?src-supplier=Conrad
#define MAX_WIPER_VALUE (256.0f)
#define MIN_WIPER_VALUE (0.0f)
#define 
#define RPUMP_WIPER_REGISTER (0x0)
typedef enum
{
    RPUMP_WRITE_CMD = 0x00,
    RPUMP_INC_CMD   = 0x01,
    RPUMP_DEC_CMD   = 0x02,
    RPUMP_READ_CMD  = 0x03,
} RPUMP_CMD;

static ExitCode pump_write(const uint8_t data){

    struct pump_write_command
    {
        // byte 1
        uint8_t   _padding : 2;
        RPUMP_CMD cmd : 2;
        uint8_t   addr : 4;
        // byte 2
        uint8_t data;
    };
    const struct pump_write_command tx_cmd = {
        .addr = RPUMP_WIPER_REGISTER,
        .cmd  = RPUMP_WRITE_CMD,
        .data = data,
    };
    static_assert(sizeof(struct pump_write_command) == 2);
    return hw_i2c_transmit(&r_pump_i2c, (const uint8_t *)&tx_cmd, sizeof(struct pump_write_command));
}

ExitCode io_rPump_isPumpReady()
{
    return hw_i2c_isTargetReady(&r_pump_i2c);
}

ExitCode io_rPump_write(const uint8_t data)
{
    struct pump_write_command
    {
        // byte 1
        uint8_t   _padding : 2;
        RPUMP_CMD cmd : 2;
        uint8_t   addr : 4;
        // byte 2
        uint8_t data;
    };
    const struct pump_write_command tx_cmd = {
        .addr = RPUMP_WIPER_REGISTER,
        .cmd  = RPUMP_WRITE_CMD,
        .data = data,
    };
    static_assert(sizeof(struct pump_write_command) == 2);
    return hw_i2c_transmit(&r_pump_i2c, (const uint8_t *)&tx_cmd, sizeof(struct pump_write_command));
}

ExitCode io_rPump_read(uint8_t *dest)
{
    return hw_i2c_memoryRead(&r_pump_i2c, RPUMP_WIPER_REGISTER, dest, sizeof(dest));
}

bool io_rPump_setPercentage(float percentage)
{
    percentage = CLAMP(percentage, 0.0f, 100.0f);
    return io_rPump_write((uint8_t)((1.0f - percentage / 100.0f) * MAX_WIPER_VALUE));
}

bool io_rPump_readPercentage(float *dest)
{
    uint8_t data;
    RETURN_IF_ERR(io_rPump_read(&data));
    *dest = (1.0f - (float)data / MAX_WIPER_VALUE) * 100.0f;
    return true;
}