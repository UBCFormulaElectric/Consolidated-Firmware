#include "io_potentiometer.h"
#include "utils.h"
#include "hw_utils.h"
#include "hw_i2cs.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_WIPER_VALUE (256.0f)
#define MIN_WIPER_VALUE (0.0f)

#define POT_WIPER0_REGISTER (0x0)
#define POT_WIPER1_REGISTER (0x1)

typedef enum
{
    POTENTIOMETER_WRITE_CMD = 0x00,
    POTENTIOMETER_INC_CMD   = 0x01,
    POTENTIOMETER_DEC_CMD   = 0x02,
    POTENTIOMETER_READ_CMD  = 0x03,
} POTENTIOMETER_CMD;

typedef struct __attribute__((packed))
{
    // byte 1
    uint8_t           _padding : 2;
    POTENTIOMETER_CMD cmd : 2;
    uint8_t           addr : 4;
    // byte 2
    uint8_t data;
} pump_write_command;

ExitCode io_potentiometer_readPercentage(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t *dest)
{
    uint8_t data[2];
    RETURN_IF_ERR(io_potentiometer_readWiper(potentiometer, wiper, data));
    uint16_t read_data = (uint16_t)(data[0] << 8 | data[1]);
    *dest              = (uint8_t)(read_data * 100 / MAX_WIPER_VALUE);
    return EXIT_CODE_OK;
}

ExitCode
    io_potentiometer_writePercentage(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t percentage)
{
    return io_potentiometer_writeWiper(potentiometer, wiper, (uint8_t)((percentage / 100.0f) * MAX_WIPER_VALUE));
}

ExitCode io_potentiometer_readWiper(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t dest[2])
{
    uint8_t read_cmd = (uint8_t)(((wiper == WIPER0) ? POT_WIPER0_REGISTER : POT_WIPER1_REGISTER) << 4) |
                       (uint8_t)POTENTIOMETER_READ_CMD << 2;
    RETURN_IF_ERR(hw_i2c_transmit(potentiometer->i2c_handle, &read_cmd, sizeof(read_cmd)));
    return hw_i2c_receive(potentiometer->i2c_handle, dest, 2);
}

ExitCode io_potentiometer_writeWiper(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t data)
{
    assert(wiper == WIPER0 || wiper == WIPER1);

    const pump_write_command tx_cmd = (pump_write_command){
        .addr = (uint8_t)(((wiper == WIPER0) ? POT_WIPER0_REGISTER : POT_WIPER1_REGISTER) & 0xF),
        .cmd  = POTENTIOMETER_WRITE_CMD,
        .data = data,
    };
    return hw_i2c_transmit(potentiometer->i2c_handle, (uint8_t *)&tx_cmd, sizeof(tx_cmd));
}