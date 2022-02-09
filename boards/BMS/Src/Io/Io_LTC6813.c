#include <assert.h>
#include <stdbool.h>
#include "main.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

// Time that a blocking SPI transaction should wait for until until an error is
// returned
#define SPI_TIMEOUT_MS (20U)

// TODO: Determine the ADC conversion timeout threshold #674
// Driver will send a command to check if ADC conversions are finished on the
// LTC6813 up to 10 times
#define MAX_NUM_OF_CONV_COMPLETE_CHECKS (10U)

// Command used to start ADC conversions
#define ADCV ((0x260 + (MD << 7) + (DCP << 4) + CH))

// Command used to poll ADC conversions
#define PLADC (0x0714U)
#define PLADC_RX_SIZE (1U)
#define ADC_CONV_INCOMPLETE (0xFFU)

// Command and parameters to write to configuration register A
#define WRCFGA (0x0001U)
#define VUV (0x4E1U)
#define VOV (0x8CAU)
#define ADCOPT (1U)
#define REFON (0U)
#define DTEN (0U)
#define DEFAULT_CFGRA_CONFIG                                                 \
    {                                                                        \
        [LTC6813_REG_0] = (0x1F << 3) + (REFON << 2) + (DTEN << 1) + ADCOPT, \
        [LTC6813_REG_1] = (uint8_t)VUV,                                      \
        [LTC6813_REG_2] = ((VOV & 0xF) << 4) + (VUV >> 8),                   \
        [LTC6813_REG_3] = (VOV >> 4), [LTC6813_REG_4] = 0x0000U,             \
        [LTC6813_REG_5]                               = 0x0000U,             \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_0] = 0U,                  \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_1] = 0U,                  \
    }

#define DEFAULT_CFGRA_CONFIG_                                             \
    {                                                                     \
        [LTC6813_REG_0] = 0U, [LTC6813_REG_1] = 0U, [LTC6813_REG_2] = 0U, \
        [LTC6813_REG_3] = 0U, [LTC6813_REG_4] = 0x0U,                     \
        [LTC6813_REG_5]                               = 0x0000U,          \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_0] = 0U,               \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_1] = 0U,               \
    }

#define WRCFGB (0x0024U)
#define REG0_ON (0xF0U)
#define REG0_OFF (0x00U)
#define REG1_ON (0x07U)
#define REG1_OFF (0x00U)
#define DEFAULT_CFGRB_CONFIG                                              \
    {                                                                     \
        [LTC6813_REG_0] = REG0_OFF + 0x000FU, [LTC6813_REG_1] = REG1_OFF, \
        [LTC6813_REG_2] = 0U, [LTC6813_REG_3] = 0U, [LTC6813_REG_5] = 0U, \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_0] = 0U,               \
        [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_1] = 0U,               \
    }

typedef struct
{
    const uint16_t cmd;
    uint8_t        config_register[NUM_REG_GROUP_BYTES];
} ConfigurationRegisters_S;

static ConfigurationRegisters_S config_register[NUM_OF_CONFIG_REGISTERS] =
{
    [CONFIG_REGISTER_A] =
    {
        .cmd = WRCFGA,
        .config_register =
        {
            [LTC6813_REG_0] = (REFON << 2) + (DTEN << 1) + ADCOPT,
            [LTC6813_REG_1] = (uint8_t)VUV,
            [LTC6813_REG_2] = ((VOV & 0xF) << 4) + (VUV >> 8),
            [LTC6813_REG_3] = (VOV >> 4),
            [LTC6813_REG_4] = 0x0U,
            [LTC6813_REG_5] = 0x0U,
            [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_0] = 0U,
            [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_1] = 0U,
        }
    },
    [CONFIG_REGISTER_B] =
    {
        .cmd = WRCFGB,
        .config_register =
        {
            [LTC6813_REG_0] = REG0_OFF,
            [LTC6813_REG_1] = REG1_OFF,
            [LTC6813_REG_2] = 0U,
            [LTC6813_REG_3] = 0U,
            [LTC6813_REG_4] = 0U,
            [LTC6813_REG_5] = 0U,
            [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_0] = 0U,
            [NUM_OF_REGS_IN_GROUP + LTC6813_PEC15_BYTE_1] = 0U,
        }
    }
};

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

void Io_LTC6813_InitSpiHandle(SPI_HandleTypeDef *spi_handle)
{
    assert(spi_handle != NULL);

    ltc6813_spi = Io_SharedSpi_Create(
        spi_handle, SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, SPI_TIMEOUT_MS);
}

uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint8_t size)
{
    // Initialize the value of the PEC15 remainder to 16
    uint16_t pec15_remainder = 16U;
    uint8_t  pec15_lut_index = 0U;

    // Refer to PEC15 calculation in the 'PEC Calculation' of the LTC6813
    // datasheet
    for (size_t i = 0U; i < size; i++)
    {
        pec15_lut_index =
            ((uint8_t)(pec15_remainder >> 7) ^ data_buffer[i]) & 0xFF;
        pec15_remainder =
            (uint16_t)((pec15_remainder << 8) ^ crc[pec15_lut_index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(pec15_remainder << 1);
}

void Io_LTC6813_PackPec15(uint8_t *tx_data, uint8_t size)
{
    const uint16_t cfg_reg_a_pec15 = Io_LTC6813_CalculatePec15(tx_data, size);
    tx_data[size + LTC6813_PEC15_BYTE_0] = (uint8_t)(cfg_reg_a_pec15 >> 8);
    tx_data[size + LTC6813_PEC15_BYTE_1] = (uint8_t)cfg_reg_a_pec15;
}

bool Io_LTC6813_SendCommand(uint16_t cmd)
{
    uint8_t tx_cmd[NUM_TX_CMD_BYTES] = { 0U };
    Io_LTC6813_PackCmd(tx_cmd, cmd);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    return Io_SharedSpi_Transmit(ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES);
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
    uint8_t tx_cmd[NUM_TX_CMD_BYTES] = { 0U };
    Io_LTC6813_PackCmd(tx_cmd, PLADC);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    // All chips on the daisy chain have finished converting cell voltages when
    // data read back = 0xFF.
    while (rx_data == ADC_CONV_INCOMPLETE)
    {
        const bool is_status_ok = Io_SharedSpi_TransmitAndReceive(
            ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES, &rx_data, PLADC_RX_SIZE);

        if (!is_status_ok || (num_attempts >= MAX_NUM_OF_CONV_COMPLETE_CHECKS))
        {
            status = false;
            break;
        }

        num_attempts++;
    }

    return status;
}

#include <string.h>
bool Io_LTC6813_ConfigureRegisterA()
{
    bool status = false;

    // Prepare write configuration register A command
    uint8_t tx_cmd[NUM_TX_CMD_BYTES + NUM_REG_GROUP_BYTES] = { 0U };
    Io_LTC6813_PackCmd(tx_cmd, WRCFGA);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    // Prepare data to write to configuration register A
    uint8_t tx_cfg[NUM_REG_GROUP_BYTES] = DEFAULT_CFGRA_CONFIG;
    Io_LTC6813_PackPec15(tx_cfg, NUM_OF_REGS_IN_GROUP);
    memcpy(&tx_cmd[4], tx_cfg, NUM_REG_GROUP_BYTES);

    status = Io_SharedSpi_Transmit(
        ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES + NUM_REG_GROUP_BYTES);

    // Write default configs to configuration register A to all devices
    // connected to the daisy chain
    // Io_SharedSpi_SetNssLow(ltc6813_spi);
    // if (Io_SharedSpi_TransmitWithoutNssToggle(
    //        ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES))
    //{
    //    status = Io_SharedSpi_TransmitWithoutNssToggle(
    //        ltc6813_spi, tx_cfg, NUM_REG_GROUP_BYTES);
    //}
    // Io_SharedSpi_SetNssHigh(ltc6813_spi);

    return status;
}

bool Io_LTC6813_ConfigureRegisterB(void)
{
    bool status = false;

    // Prepare write configuration register A command
    uint8_t tx_cmd[NUM_TX_CMD_BYTES] = { 0U };
    Io_LTC6813_PackCmd(tx_cmd, WRCFGB);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    // Prepare data to write to configuration register A
    uint8_t tx_cfg[NUM_REG_GROUP_BYTES] = DEFAULT_CFGRB_CONFIG;
    Io_LTC6813_PackPec15(tx_cfg, NUM_OF_REGS_IN_GROUP);

    // Write default configs to configuration register A to all devices
    // connected to the daisy chain
    Io_SharedSpi_SetNssLow(ltc6813_spi);

    if (Io_SharedSpi_TransmitWithoutNssToggle(
            ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES))
    {
        status = Io_SharedSpi_MultipleTransmitWithoutNssToggle(
            ltc6813_spi, tx_cfg, NUM_REG_GROUP_BYTES,
            NUM_OF_ACCUMULATOR_SEGMENTS);
    }

    Io_SharedSpi_SetNssHigh(ltc6813_spi);

    return status;
}

bool Io_LTC6813_ConfigureRegister(ConfigurationRegisters_E cfg_reg)
{
    bool status = false;

    // Prepare write configuration register command
    uint8_t tx_cmd[NUM_TX_CMD_BYTES] = { 0U };
    Io_LTC6813_PackCmd(tx_cmd, config_register[cfg_reg].cmd);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    // Prepare data to write to configuration register
    Io_LTC6813_PackPec15(
        config_register[cfg_reg].config_register, NUM_OF_REGS_IN_GROUP);

    // Write default configs to configuration register to all devices
    // connected to the daisy chain
    Io_SharedSpi_SetNssLow(ltc6813_spi);

    if (Io_SharedSpi_TransmitWithoutNssToggle(
            ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES))
    {
        status = Io_SharedSpi_MultipleTransmitWithoutNssToggle(
            ltc6813_spi, config_register[cfg_reg].config_register,
            NUM_REG_GROUP_BYTES, NUM_OF_ACCUMULATOR_SEGMENTS);
    }

    Io_SharedSpi_SetNssHigh(ltc6813_spi);

    return status;
}

void Io_LTC6813_EnterReadyState(void)
{
    uint8_t rx_data;

    // Generate isoSPI traffic to wake up the daisy chain by sending a command
    // to read a single byte NUM_OF_LTC6813 times.
    for (size_t i = 0U; i < NUM_OF_ACCUMULATOR_SEGMENTS; i++)
    {
        Io_SharedSpi_Receive(ltc6813_spi, &rx_data, 1U);
    }
}
