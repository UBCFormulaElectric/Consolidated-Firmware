#include "io/ltc6813/ltc6813_internal.h"

#include <stdint.h>
#include <string.h>

#include "hw/spis.h"

typedef struct
{
    uint16_t cmd;
    uint16_t pec;
} CmdPayload;

typedef struct
{
    uint16_t regs[REGS_PER_GROUP];
    uint16_t pec;
} RegGroupPayload;

static_assert(sizeof(RegGroupPayload) == REGISTER_GROUP_SIZE + PEC_SIZE);

static uint16_t swapEndianness(uint16_t value)
{
    return (uint16_t)((value >> 8) | (value << 8));
}

static uint16_t calculatePec15(const uint8_t *data, const uint8_t len)
{
    static const uint16_t pec15_lut[256] = {
        0x0,    0xC599, 0xCEAB, 0xB32,  0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407, 0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951,
        0xE263, 0x27FA, 0xAD97, 0x680E, 0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B, 0x52A2,
        0x815F, 0x44C6, 0x4FF4, 0x8A6D, 0x5B2E, 0x9EB7, 0x9585, 0x501C, 0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0,
        0x6182, 0xA41B, 0x77E6, 0xB27F, 0xB94D, 0x7CD4, 0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF, 0xE0DD, 0x2544,
        0x2BE,  0xC727, 0xCC15, 0x98C,  0xDA71, 0x1FE8, 0x14DA, 0xD143, 0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93,
        0xE5A1, 0x2038, 0x7C2,  0xC25B, 0xC969, 0xCF0,  0xDF0D, 0x1A94, 0x11A6, 0xD43F, 0x5E52, 0x9BCB, 0x90F9, 0x5560,
        0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC, 0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8, 0xA8EB, 0x6D72,
        0x6640, 0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE, 0x8423, 0x41BA, 0x4A88, 0x8F11,
        0x57C,  0xC0E5, 0xCBD7, 0xE4E,  0xDDB3, 0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D,
        0xE71F, 0x2286, 0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477, 0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26,
        0x8EDB, 0x4B42, 0x4070, 0x85E9, 0xF84,  0xCA1D, 0xC12F, 0x4B6,  0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83, 0x3E1A,
        0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E, 0xF93D, 0x3CA4, 0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0,
        0xD3A,  0xC8A3, 0xC391, 0x608,  0xD5F5, 0x106C, 0x1B5E, 0xDEC7, 0x54AA, 0x9133, 0x9A01, 0x5F98, 0x8C65, 0x49FC,
        0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862, 0xBDFB, 0xB6C9, 0x7350, 0x51D6, 0x944F, 0x9F7D, 0x5AE4,
        0x8919, 0x4C80, 0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5, 0x762C, 0xFC41, 0x39D8,
        0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC, 0x846,  0xCDDF, 0xC6ED, 0x374,  0xD089, 0x1510, 0x1E22, 0xDBBB,
        0xAF8,  0xCF61, 0xC453, 0x1CA,  0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66, 0x3054, 0xF5CD, 0x2630, 0xE3A9,
        0xE89B, 0x2D02, 0xA76F, 0x62F6, 0x69C4, 0xAC5D, 0x7fA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A,
        0x8BA7, 0x4E3E, 0x450C, 0x8095
    };

    // Initialize the value of the PEC15 remainder to 16
    uint16_t remainder = 16U;
    uint8_t  index     = 0U;

    // Refer to PEC15 calculation in the 'PEC Calculation' of the LTC6813
    // datasheet
    for (size_t i = 0U; i < len; i++)
    {
        index     = ((uint8_t)(remainder >> 7U) ^ data[i]) & 0xFF;
        remainder = (uint16_t)(remainder << 8U ^ pec15_lut[index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(remainder << 1);
}

static uint16_t buildDataPec(const uint8_t *data, const uint8_t len)
{
    // PEC15 needs to be sent as big-endian.
    return swapEndianness(calculatePec15(data, len));
}

static bool checkPec(const uint8_t *data, const uint8_t len, const uint16_t pec)
{
    const uint16_t expected = buildDataPec(data, len);
    return expected == pec;
}

CmdPayload buildTxCmd(const uint16_t command)
{
    // Command needs to be sent as big-endian.
    CmdPayload out = { .cmd = swapEndianness(command) };
    out.pec        = buildDataPec((uint8_t *)&out, sizeof(uint16_t));
    return out;
}

ExitCode io_ltc6813_sendCommand(const uint16_t command)
{
    const CmdPayload tx_cmd = buildTxCmd(command);
    return hw_spi_transmit(&ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd));
}

ExitCode io_ltc6813_poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len)
{
    const CmdPayload tx_cmd = buildTxCmd(cmd);
    return hw_spi_transmitThenReceive(&ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), poll_buf, poll_buf_len);
}

void io_ltc6813_readRegGroup(
    uint16_t cmd,
    uint16_t regs[NUM_SEGMENTS][REGS_PER_GROUP],
    ExitCode comm_success[NUM_SEGMENTS])
{
    memset(regs, 0, NUM_SEGMENTS * REGS_PER_GROUP * sizeof(uint16_t));

    const CmdPayload tx_cmd = buildTxCmd(cmd);
    RegGroupPayload  rx_buffer[NUM_SEGMENTS];
    const ExitCode   tx_status = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));

    if (IS_EXIT_ERR(tx_status))
    {
        for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
        {
            comm_success[segment] = tx_status;
        }

        return;
    }

    for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
    {
        const RegGroupPayload *seg_reg_group = &rx_buffer[seg_idx];

        if (!checkPec((uint8_t *)seg_reg_group, REGISTER_GROUP_SIZE, seg_reg_group->pec))
        {
            comm_success[seg_idx] = EXIT_CODE_CHECKSUM_FAIL;
            continue;
        }

        memcpy(regs[seg_idx], seg_reg_group->regs, REGS_PER_GROUP * sizeof(uint16_t));
        comm_success[seg_idx] = EXIT_CODE_OK;
    }
}

ExitCode io_ltc6813_writeRegGroup(uint16_t cmd, uint16_t regs[NUM_SEGMENTS][REGS_PER_GROUP])
{
    struct
    {
        CmdPayload      tx_cmd;
        RegGroupPayload payload[NUM_SEGMENTS];
    } tx_buffer;
    static_assert(sizeof(tx_buffer) == (CMD_SIZE + PEC_SIZE) + ((REGISTER_GROUP_SIZE + PEC_SIZE) * NUM_SEGMENTS));

    tx_buffer.tx_cmd = buildTxCmd(cmd);

    for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
    {
        // Data used to configure the last segment on the daisy chain needs to be sent first
        RegGroupPayload *seg_reg_group = &tx_buffer.payload[(NUM_SEGMENTS - 1) - seg_idx];

        memcpy(seg_reg_group->regs, regs[seg_idx], REGS_PER_GROUP * sizeof(uint16_t));
        seg_reg_group->pec = buildDataPec((uint8_t *)regs[seg_idx], REGS_PER_GROUP * sizeof(uint16_t));
    }

    return hw_spi_transmit(&ltc6813_spi_ls, (uint8_t *)&tx_buffer, sizeof(tx_buffer));
}
