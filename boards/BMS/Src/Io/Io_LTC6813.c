#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "main.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

// Time that a SPI transaction should wait for until until an error is returned
#define SPI_TIMEOUT_MS (20U)

// Max number of commands to check for completed ADC conversions
#define MAX_NUM_OF_CONV_COMPLETE_CHECKS (10U)

// Command used to start ADC conversions
#define ADCV ((0x260 + (MD << 7) + (DCP << 4) + CH))

// Command used to poll ADC conversions
#define PLADC (0x0714U)
#define PLADC_RX_SIZE (1U)
#define ADC_CONV_INCOMPLETE (0xFFU)

#define NUM_CMD_BYTES (2U)

#define DEFAULT_CFGRA_CONFIG                                                \
    {                                                                       \
        [REG_GROUP_0] =                                                     \
            (uint8_t)(ENABLE_ALL_CFGRA_GPIO | REFON | DTEN | ADCOPT),       \
        [REG_GROUP_1] = (uint8_t)VUV,                                       \
        [REG_GROUP_2] = (uint8_t)((VOV & 0xF) << 4) + (VUV >> 8),           \
        [REG_GROUP_3] = (uint8_t)(VOV >> 4), [REG_GROUP_4] = 0x0U,          \
        [REG_GROUP_5] = 0x0U, [REG_GROUP_PEC0] = 0U, [REG_GROUP_PEC1] = 0U, \
    }

#define DEFAULT_CFGRB_CONFIG                                              \
    {                                                                     \
        [REG_GROUP_0] = ENABLE_ALL_CFGRB_GPIO, [REG_GROUP_1] = 0U,        \
        [REG_GROUP_2] = 0U, [REG_GROUP_3] = 0U, [REG_GROUP_4] = 0U,       \
        [REG_GROUP_5] = 0U, [REG_GROUP_PEC0] = 0U, [REG_GROUP_PEC1] = 0U, \
    }

// Macros for setting DCC bits
#define SET_ALL_DCC_BITS (0xFFFFU)
#define SET_MIN_CELL_DCC_BIT(index) ((uint16_t)(1U << (index)))
#define SET_CFGRA4_DCC_BITS(dcc_bits) (0xFFU & (uint8_t)(dcc_bits))
#define SET_CFGRA5_DCC_BITS(dcc_bits) (0x0FU & (uint8_t)((dcc_bits) >> 8U))
#define SET_CFGRB0_DCC_BITS(dcc_bits) (0xF0U & (uint8_t)((dcc_bits) >> 8U))

extern struct SharedSpi *ltc6813_spi;
struct SharedSpi *       ltc6813_spi = NULL;

// CRC check LUT
static const uint16_t crc[UINT8_MAX + 1] = {
    0x0,    0xC599, 0xCEAB, 0xB32,  0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407,
    0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951, 0xE263, 0x27FA, 0xAD97, 0x680E,
    0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B,
    0x52A2, 0x815F, 0x44C6, 0x4FF4, 0x8A6D, 0x5B2E, 0x9EB7, 0x9585, 0x501C,
    0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0, 0x6182, 0xA41B, 0x77E6,
    0xB27F, 0xB94D, 0x7CD4, 0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF,
    0xE0DD, 0x2544, 0x2BE,  0xC727, 0xCC15, 0x98C,  0xDA71, 0x1FE8, 0x14DA,
    0xD143, 0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93, 0xE5A1, 0x2038,
    0x7C2,  0xC25B, 0xC969, 0xCF0,  0xDF0D, 0x1A94, 0x11A6, 0xD43F, 0x5E52,
    0x9BCB, 0x90F9, 0x5560, 0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC,
    0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8, 0xA8EB, 0x6D72, 0x6640,
    0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE,
    0x8423, 0x41BA, 0x4A88, 0x8F11, 0x57C,  0xC0E5, 0xCBD7, 0xE4E,  0xDDB3,
    0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D,
    0xE71F, 0x2286, 0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477,
    0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26, 0x8EDB, 0x4B42, 0x4070, 0x85E9,
    0xF84,  0xCA1D, 0xC12F, 0x4B6,  0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83,
    0x3E1A, 0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E, 0xF93D, 0x3CA4,
    0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0, 0xD3A,  0xC8A3, 0xC391,
    0x608,  0xD5F5, 0x106C, 0x1B5E, 0xDEC7, 0x54AA, 0x9133, 0x9A01, 0x5F98,
    0x8C65, 0x49FC, 0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862,
    0xBDFB, 0xB6C9, 0x7350, 0x51D6, 0x944F, 0x9F7D, 0x5AE4, 0x8919, 0x4C80,
    0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5,
    0x762C, 0xFC41, 0x39D8, 0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC,
    0x846,  0xCDDF, 0xC6ED, 0x374,  0xD089, 0x1510, 0x1E22, 0xDBBB, 0xAF8,
    0xCF61, 0xC453, 0x1CA,  0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66,
    0x3054, 0xF5CD, 0x2630, 0xE3A9, 0xE89B, 0x2D02, 0xA76F, 0x62F6, 0x69C4,
    0xAC5D, 0x7fA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A,
    0x8BA7, 0x4E3E, 0x450C, 0x8095
};

static inline void Io_ConfigureRegisterA(uint8_t *tx_cmd, uint8_t *tx_cfg)
{
    // Pack command to write to configure register A
    Io_LTC6813_PrepareCmd(tx_cmd, WRCFGA);
    // Pack data to write to configuration register A
    uint8_t cfgra[NUM_REG_GROUP_BYTES] = DEFAULT_CFGRA_CONFIG;
    memcpy(tx_cfg, cfgra, NUM_REG_GROUP_BYTES);
}

static inline void Io_ConfigureRegisterB(
    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES],
    uint8_t tx_cfg[NUM_REG_GROUP_BYTES])
{
    // Pack command to write to configure register A
    Io_LTC6813_PrepareCmd(tx_cmd, WRCFGB);
    // Pack data to write to configuration register A
    uint8_t cfgrb[NUM_REG_GROUP_BYTES] = DEFAULT_CFGRB_CONFIG;
    memcpy(tx_cfg, cfgrb, NUM_REG_GROUP_BYTES);
}

static inline bool Io_WriteConfigurationRegister(
    Ltc6813CfgRegs_E cfg_reg,
    uint8_t *        tx_cmd,
    uint8_t *        tx_cfg,
    uint8_t          min_cell_segment,
    uint16_t         min_cell_dcc_bits)
{
    bool status = true;

    // Configurations for the last segment need to be sent first on the daisy
    // chain
    Io_SharedSpi_SetNssLow(ltc6813_spi);
    if (Io_SharedSpi_TransmitWithoutNssToggle(
            ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES))
    {
        for (uint8_t curr_segment = 0U;
             curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
        {
            uint16_t dcc_bits = (curr_segment == NUM_OF_ACCUMULATOR_SEGMENTS -
                                                     min_cell_segment - 1)
                                    ? min_cell_dcc_bits
                                    : SET_ALL_DCC_BITS;

            if (cfg_reg == CONFIG_REG_A)
            {
                tx_cfg[REG_GROUP_4] = SET_CFGRA4_DCC_BITS(dcc_bits);
                tx_cfg[REG_GROUP_5] = SET_CFGRA5_DCC_BITS(dcc_bits);
            }
            if (cfg_reg == CONFIG_REG_B)
            {
                tx_cfg[REG_GROUP_0] = SET_CFGRB0_DCC_BITS(dcc_bits);
            }

            Io_LTC6813_PackPec15(tx_cfg, NUM_OF_REGS_IN_GROUP);
            if (!Io_SharedSpi_TransmitWithoutNssToggle(
                    ltc6813_spi, tx_cfg, NUM_REG_GROUP_BYTES))
            {
                status = false;
                break;
            }

            if (cfg_reg == CONFIG_REG_A)
            {
                tx_cfg[REG_GROUP_4] = 0U;
                tx_cfg[REG_GROUP_5] = 0U;
            }
            else if (cfg_reg == CONFIG_REG_B)
            {
                tx_cfg[REG_GROUP_0] = ENABLE_ALL_CFGRB_GPIO;
                tx_cfg[REG_GROUP_1] = 0U;
            }
        }
    }

    Io_SharedSpi_SetNssHigh(ltc6813_spi);

    return status;
}

bool Io_LTC6813_SetConfigurationRegister(
    Ltc6813CfgRegs_E cfg_reg,
    uint8_t          min_cell_segment,
    uint16_t         min_cell_loc)
{
    uint8_t        tx_cmd[TOTAL_NUM_CMD_BYTES] = { 0U };
    uint8_t        tx_cfg[NUM_REG_GROUP_BYTES] = { 0U };
    const uint16_t min_cell_dcc_bits =
        (uint16_t)(~(SET_MIN_CELL_DCC_BIT(min_cell_loc)));

    switch (cfg_reg)
    {
        case CONFIG_REG_A:
            Io_ConfigureRegisterA(tx_cmd, tx_cfg);
            break;

        case CONFIG_REG_B:
            Io_ConfigureRegisterB(tx_cmd, tx_cfg);
            break;

        default:
            break;
    }

    return Io_WriteConfigurationRegister(
        cfg_reg, tx_cmd, tx_cfg, min_cell_segment, min_cell_dcc_bits);
}

void Io_LTC6813_InitSpiHandle(SPI_HandleTypeDef *spi_handle)
{
    assert(spi_handle != NULL);

    ltc6813_spi = Io_SharedSpi_Create(
        spi_handle, SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, SPI_TIMEOUT_MS);
}

void Io_LTC6813_PrepareCmd(uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES], uint16_t cmd)
{
    // tx_cmd[CMD_BYTE_0] = (uint8_t)(cmd >> 8);
    // tx_cmd[CMD_BYTE_1] = (uint8_t)cmd;

    Io_LTC6813_PackWordInBytes(tx_cmd, cmd);
    Io_LTC6813_PackPec15(tx_cmd, NUM_CMD_BYTES);
}

static uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint8_t size)
{
    // Initialize the value of the PEC15 remainder to 16
    uint16_t pec15_remainder = 16U;
    uint8_t  pec15_lut_index = 0U;

    // Refer to PEC15 calculation in the 'PEC Calculation' of the LTC6813
    // datasheet
    for (size_t i = 0U; i < size; i++)
    {
        pec15_lut_index =
            ((uint8_t)(pec15_remainder >> 7U) ^ data_buffer[i]) & 0xFFU;
        pec15_remainder =
            (uint16_t)((pec15_remainder << 8U) ^ crc[pec15_lut_index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(pec15_remainder << 1);
}

uint16_t Io_LTC6813_CalculateRegGroupPec15(uint8_t *data_buffer)
{
    return Io_LTC6813_CalculatePec15(data_buffer, NUM_OF_REGS_IN_GROUP);
}

void Io_LTC6813_PackPec15(uint8_t *tx_data, uint8_t size)
{
    const uint16_t cfg_reg_a_pec15 = Io_LTC6813_CalculatePec15(tx_data, size);
    Io_LTC6813_PackWordInBytes(&tx_data[size + PEC15_BYTE_0], cfg_reg_a_pec15);
}

bool Io_LTC6813_SendCommand(uint16_t cmd)
{
    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES] = { 0U };
    Io_LTC6813_PrepareCmd(tx_cmd, cmd);

    return Io_SharedSpi_Transmit(ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES);
}

bool Io_LTC6813_StartADCConversion(void)
{
    return Io_LTC6813_SendCommand(ADCV);
}

bool Io_LTC6813_PollAdcConversions(void)
{
    bool    status       = true;
    uint8_t num_attempts = 0U;
    uint8_t rx_data      = ADC_CONV_INCOMPLETE;

    // Get the status of ADC conversions
    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES] = { 0U };
    Io_LTC6813_PrepareCmd(tx_cmd, PLADC);

    // All chips on the daisy chain have finished converting cell voltages when
    // data read back = 0xFF.
    while (rx_data == ADC_CONV_INCOMPLETE)
    {
        const bool is_status_ok = Io_SharedSpi_TransmitAndReceive(
            ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES, &rx_data, PLADC_RX_SIZE);

        if (!is_status_ok || (num_attempts >= MAX_NUM_OF_CONV_COMPLETE_CHECKS))
        {
            status = false;
            break;
        }

        num_attempts++;
    }

    return status;
}

bool Io_LTC6813_ConfigureRegisterA()
{
    return true;
}

bool Io_LTC6813_ConfigureRegisterB(void)
{
    return true;
}
