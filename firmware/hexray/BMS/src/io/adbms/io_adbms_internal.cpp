#include "io_adbms_internal.hpp"

#include "hw_spi.h"

struct CmdPayload
{
    uint16_t cmd{ 0 };
    uint16_t pec{ 0 };
};

// Polynomial mask (polynomial without the x^15 term) for
// x^15 + x^14 + x^10 + x^8 + x^7 + x^4 + x^3 + 1
// bits positions: 14,10,8,7,4,3,0 -> mask = 0x4599
#define PEC15_POLY_MASK 0x4599
#define PEC15_INIT 0x0010 // initial remainder (15-bit)

uint16_t calculateCommandPec(uint16_t command)
{
    uint16_t remainder = PEC15_INIT & 0x7FFF; // 15-bit LFSR
    uint16_t poly      = PEC15_POLY_MASK & 0x7FFF;

    // Process 16 bits MSB-first
    for (int bit = 15; bit >= 0; --bit)
    {
        uint8_t data_bit = (command >> bit) & 1;  // next input bit
        uint8_t msb      = (remainder >> 14) & 1; // bit leaving the register

        remainder = ((remainder << 1) & 0x7FFF) | data_bit; // shift in new bit
        if (msb)
            remainder ^= poly; // XOR with generator mask if top bit was 1
    }

    // Left-shift once so LSB = 0 (device transmits 16 bits, lowest bit unused)
    return (remainder << 1) & 0xFFFE;
}

CmdPayload buildTxCmd(const uint16_t command)
{
    CmdPayload out = { .cmd = command, .pec = calculateCommandPec(command) };
    return out;
}

namespace io::adbms
{
ExitCode io_adbms_sendCommand(const uint16_t command)
{
    const CmdPayload tx_cmd = buildTxCmd(command);
    return hw_spi_transmit(&adbms_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd));
}

ExitCode io_adbms_poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len)
{
    const CmdPayload tx_cmd = buildTxCmd(cmd);
    return hw_spi_transmitThenReceive(&adbms_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), poll_buf, poll_buf_len);
}

void io_adbms_readRegGroup(
    uint16_t cmd,
    uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP],
    ExitCode comm_success[io::NUM_SEGMENTS])
{
    const CmdPayload tx_cmd = buildTxCmd(cmd);
    RegGroupPayload  rx_buffer[io::NUM_SEGMENTS];
}

ExitCode io_adbms_writeRegGroup(uint16_t cmd, uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP])
{
    return EXIT_CODE_OK; //stub
}

} // namespace io::adbms
