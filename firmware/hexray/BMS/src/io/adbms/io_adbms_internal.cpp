#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"

#include "hw_spi.h"

struct CmdPayload
{
    uint16_t cmd;
    uint16_t pec15;
};

struct RegGroupPayload
{
    uint16_t regs[io::adbms::REGS_PER_GROUP];
    uint16_t pec10;
};

// Polynomial mask (polynomial without the x^15 term) for
// x^15 + x^14 + x^10 + x^8 + x^7 + x^4 + x^3 + 1
// bits positions: 14,10,8,7,4,3,0 -> mask = 0x4599
#define PEC15_POLY_MASK 0x4599
#define PEC15_INIT 0x0010 // initial remainder (15-bit)

static uint16_t calculatePec15(uint16_t command)
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

// Polynomial mask for PEC10 (polynomial without the x^10 term)
// x^10 + x^7 + x^3 + x^2 + x + 1
// bit positions: 7,3,2,1,0 -> mask = 0x8F
#define PEC10_POLY_MASK 0x008F
// Initial remainder (10-bit)
#define PEC10_INIT 0x0010
// Mask to keep PEC10 to 10 bits
#define PEC10_MASK 0x03FF

static uint16_t calculatePec10(const uint8_t* data, size_t len)
{
    uint16_t remainder = PEC10_INIT;

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t byte = data[i];
        for (int bit = 7; bit >= 0; --bit)
        {
            uint8_t data_bit = (byte >> bit) & 0x1;
            uint8_t crc_msb  = (remainder >> 9) & 0x1; // MSB of 10-bit CRC

            remainder <<= 1;
            remainder &= PEC10_MASK;

            if (crc_msb ^ data_bit)
                remainder ^= PEC10_POLY_MASK;
        }
    }

    return remainder & PEC10_MASK;
}

static CmdPayload buildTxCmd(const uint16_t command)
{
    CmdPayload out = { .cmd = command, .pec15 = calculatePec15(command) };
    return out;
}

static bool checkDataPec(const uint8_t *data, const uint8_t len, const uint16_t pec)
{
    const uint16_t expected = calculatePec10((uint8_t *)data, len);
    return expected == pec;
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
    const ExitCode   tx_status = hw_spi_transmitThenReceive(
        &adbms_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
    
    if (IS_EXIT_ERR(tx_status))
    {
        for (uint8_t segment = 0; segment < io::NUM_SEGMENTS; segment++)
        {
            comm_success[segment] = tx_status;
        }

        return;
    }

    for (uint8_t segment = 0; segment < io::NUM_SEGMENTS; segment++)
    {
        const RegGroupPayload *segment_data = &rx_buffer[segment];
        const uint8_t         *data_bytes   = (const uint8_t *)segment_data->regs;
        const size_t          data_len     = sizeof(segment_data->regs);

        if (checkDataPec(data_bytes, data_len, segment_data->pec10))
        {
            // PEC OK, copy data
            for (uint8_t reg_idx = 0; reg_idx < io::adbms::REGS_PER_GROUP; reg_idx++)
            {
                regs[segment][reg_idx] = segment_data->regs[reg_idx];
            }
            comm_success[segment] = EXIT_CODE_OK;
        }
        else
        {
            // PEC failed
            comm_success[segment] = EXIT_CODE_CHECKSUM_FAIL;
        }
    }
}

ExitCode io_adbms_writeRegGroup(uint16_t cmd, uint16_t regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP])
{
    struct
    {
        CmdPayload      tx_cmd;
        RegGroupPayload payload[io::NUM_SEGMENTS];
    } tx_buffer;

    tx_buffer.tx_cmd = buildTxCmd(cmd);

    for (uint8_t segment = 0; segment < io::NUM_SEGMENTS; segment++)
    {
        // Copy register data
        for (uint8_t reg_idx = 0; reg_idx < io::adbms::REGS_PER_GROUP; reg_idx++)
        {
            tx_buffer.payload[segment].regs[reg_idx] = regs[segment][reg_idx];
        }

        // Calculate PEC10 for this segment's data
        const uint8_t *data_bytes = (const uint8_t *)tx_buffer.payload[segment].regs;
        const size_t  data_len   = sizeof(tx_buffer.payload[segment].regs);
        tx_buffer.payload[segment].pec10 = calculatePec10(data_bytes, data_len);
    }

    return hw_spi_transmit(&adbms_spi_ls, (uint8_t *)&tx_buffer, sizeof(tx_buffer));
}

} // namespace io::adbms
