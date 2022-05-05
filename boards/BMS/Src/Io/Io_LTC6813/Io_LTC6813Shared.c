#include <string.h>
#include <assert.h>
#include "main.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813/Io_LTC6813Shared.h"
#include "Io_LTC6813/Io_LTC6813CellVoltages.h"
#include "App_Accumulator.h"
#include "App_SharedMacros.h"

// clang-format off
// Time that a SPI transaction should wait for until until an error is returned
#define SPI_TIMEOUT_MS (10U)

// Max number of commands to check for completed ADC conversions
#define MAX_NUM_ADC_COMPLETE_CHECKS (10U)

// Command used to start/stop cell discharge
#define MUTE   (0x2800U)
#define UNMUTE (0x2900U)

// Command used to write to configuration registers
#define WRCFGA (0x0100U)
#define WRCFGB (0x2400U)

// Command used to poll ADC conversions
#define PLADC               (0x1407U)
#define PLADC_RX_SIZE       (1U)
#define ADC_CONV_INCOMPLETE (0xFFU)

// Macros used to set DCC bits in the configuration register
#define SET_CFGRA4_DCC_BITS(dcc_bits)       (0xFFU & (uint8_t)(dcc_bits))
#define SET_CFGRA5_DCC_BITS(dcc_bits)       (0x0FU & (uint8_t)((dcc_bits) >> 8U))
#define SET_CFGRB0_DCC_BITS(dcc_bits)       (0xF0U & (uint8_t)((dcc_bits) >> 8U))

#define VUV                   (0x4E1U)        // Under-voltage comparison voltage, (VUV + 1) * 16 * 100uV
#define VOV                   (0x8CAU)        // Over-voltage comparison voltage, VOV * 16 * 100uV
#define ADCOPT                (1U)            // ADC mode option bit
#define REFON                 (0U << 2U)      // Select whether references are powered up
#define DTEN                  (0U << 1U)      // Select whether discharge timer is enabled
#define ENABLE_ALL_CFGRA_GPIO (0x001FU << 3U) // Enable all GPIOs corresponding to CFGRA
#define ENABLE_ALL_CFGRB_GPIO (0x000FU)       // Enable all GPIOs corresponding to CFGRB

#define PEC15_LUT_SIZE        (256U)

enum ConfigurationRegister
{
    CONFIG_REG_A = 0U,
    CONFIG_REG_B,
    NUM_OF_CFG_REGS,
};

struct LTC6813Configurations
{
    uint8_t  default_cfg_reg[NUM_REG_GROUP_PAYLOAD_BYTES];
    uint16_t cfg_reg_cmds;
};

extern struct SharedSpi *ltc6813_spi;
struct SharedSpi *       ltc6813_spi = NULL;

static struct LTC6813Configurations ltc6813_configs[NUM_OF_CFG_REGS] =
{
    [CONFIG_REG_A] =
    {
        .cfg_reg_cmds    = WRCFGA,
        .default_cfg_reg =
        {
            [REG_GROUP_BYTE_0] = (uint8_t)(ENABLE_ALL_CFGRA_GPIO | REFON | DTEN | ADCOPT),
            [REG_GROUP_BYTE_1] = (uint8_t)VUV,
            [REG_GROUP_BYTE_2] = (uint8_t)((VOV & 0xF) << 4) + (VUV >> 8),
            [REG_GROUP_BYTE_3] = (uint8_t)(VOV >> 4),
            [REG_GROUP_BYTE_4] = 0x0U,
            [REG_GROUP_BYTE_5] = 0x0U,
        },
    },
    [CONFIG_REG_B] =
    {
        .cfg_reg_cmds    = WRCFGB,
        .default_cfg_reg =
        {
            [REG_GROUP_BYTE_0] = ENABLE_ALL_CFGRB_GPIO,
            [REG_GROUP_BYTE_1] = 0U,
            [REG_GROUP_BYTE_2] = 0U,
            [REG_GROUP_BYTE_3] = 0U,
            [REG_GROUP_BYTE_4] = 0U,
            [REG_GROUP_BYTE_5] = 0U,
        },
    }
};
// clang-format on

static const uint16_t pec15_lut[PEC15_LUT_SIZE] = {
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

/**
 * Compute PEC15 bytes for a given data buffer
 * @param data_buffer The data buffer to compute PEC15 for
 * @param size The size of the data buffer payload
 * @return The PEC15 code
 */
static uint16_t Io_CalculatePec15(const uint8_t *data_buffer, uint8_t size);

/**
 * A function called to prepare data written to configuration registers
 * @param tx_cfg Buffer containing data to write to the current configuration
 * register
 * @param curr_cfg_reg The current configuration register to configure
 */
static void
    Io_PrepareCfgRegBytes(uint8_t tx_cfg[NUM_OF_ACCUMULATOR_SEGMENTS][TOTAL_NUM_REG_GROUP_BYTES], uint8_t curr_cfg_reg);

static uint16_t Io_CalculatePec15(const uint8_t *data_buffer, uint8_t size)
{
    // Initialize the value of the PEC15 remainder to 16
    uint16_t remainder = 16U;
    uint8_t  index     = 0U;

    // Refer to PEC15 calculation in the 'PEC Calculation' of the LTC6813
    // datasheet
    for (size_t i = 0U; i < size; i++)
    {
        index     = ((uint8_t)(remainder >> 7U) ^ data_buffer[i]) & 0xFFU;
        remainder = (uint16_t)((remainder << 8U) ^ pec15_lut[index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(remainder << 1);
}

static void
    Io_PrepareCfgRegBytes(uint8_t tx_cfg[NUM_OF_ACCUMULATOR_SEGMENTS][TOTAL_NUM_REG_GROUP_BYTES], uint8_t curr_cfg_reg)
{
    // TODO: We can adjust how we want to discharge cells. In the current
    // implementation, we are discharging to the lowest cell Get the min
    // cell location
    uint8_t min_cell_segment = 0U;
    uint8_t min_cell_index   = 0U;
    Io_LTC6813CellVoltages_GetMinCellVoltage(&min_cell_segment, &min_cell_index);

    // Write to the configuration registers of each segment
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
    {
        // Data used to configure the last segment on the daisy chain needs to
        // be sent first
        const uint8_t tx_cfg_idx = (uint8_t)(NUM_OF_ACCUMULATOR_SEGMENTS - curr_segment - 1);

        // Set default tx_cfg for each segment
        memcpy(&tx_cfg[tx_cfg_idx], ltc6813_configs[curr_cfg_reg].default_cfg_reg, NUM_REG_GROUP_PAYLOAD_BYTES);

        // Get dcc bits to write for the current segment. If this is the lowest
        // cell, set DCC bits for the given segment
        const uint16_t dcc_bits = Io_LTC6813CellVoltages_GetCellsToDischarge(curr_segment);

        // Write to configuration registers DCC bits
        if (curr_cfg_reg == CONFIG_REG_A)
        {
            tx_cfg[tx_cfg_idx][REG_GROUP_BYTE_4] |= SET_CFGRA4_DCC_BITS(dcc_bits);
            tx_cfg[tx_cfg_idx][REG_GROUP_BYTE_5] |= SET_CFGRA5_DCC_BITS(dcc_bits);
        }
        else
        {
            tx_cfg[tx_cfg_idx][REG_GROUP_BYTE_0] |= SET_CFGRB0_DCC_BITS(dcc_bits);
        }

        // Calculate and pack the PEC15 bytes into data to write ot the
        // configuration register
        Io_LTC6813Shared_PackRegisterGroupPec15(tx_cfg[tx_cfg_idx]);
    }
}

void Io_LTC6813Shared_InitSpiHandle(SPI_HandleTypeDef *spi_handle)
{
    assert(spi_handle != NULL);

    // Initialize the SPI interface to communicate with the LTC6813
    ltc6813_spi = Io_SharedSpi_Create(spi_handle, SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, SPI_TIMEOUT_MS);
}

uint16_t Io_LTC6813Shared_CalculateRegGroupPec15(const uint8_t *data_buffer)
{
    return CHANGE_WORD_ENDIANNESS(Io_CalculatePec15(data_buffer, NUM_REG_GROUP_PAYLOAD_BYTES));
}

void Io_LTC6813Shared_PackCmdPec15(uint16_t *tx_cmd)
{
    // Pack the PEC15 byte into tx_cmd in big endian format
    *(tx_cmd + CMD_PEC15) = CHANGE_WORD_ENDIANNESS(Io_CalculatePec15((uint8_t *)tx_cmd, CMD_SIZE_BYTES));
}

void Io_LTC6813Shared_PackRegisterGroupPec15(uint8_t *tx_cfg)
{
    *(uint16_t *)(tx_cfg + NUM_REG_GROUP_PAYLOAD_BYTES) = Io_LTC6813Shared_CalculateRegGroupPec15(tx_cfg);
}

bool Io_LTC6813Shared_SendCommand(uint16_t cmd)
{
    uint16_t tx_cmd[NUM_OF_CMD_WORDS] = { [CMD_WORD] = cmd, [CMD_PEC15] = 0U };
    Io_LTC6813Shared_PackCmdPec15(tx_cmd);

    return Io_SharedSpi_Transmit(ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES);
}

bool Io_LTC6813Shared_PollAdcConversions(void)
{
    uint8_t num_attempts = 0U;
    uint8_t rx_data      = ADC_CONV_INCOMPLETE;

    // Prepare command to get the status of ADC conversions
    uint16_t tx_cmd[NUM_OF_CMD_WORDS] = { [CMD_WORD] = PLADC, [CMD_PEC15] = 0U };
    Io_LTC6813Shared_PackCmdPec15(tx_cmd);

    // All chips on the daisy chain have finished converting cell voltages when
    // data read back != 0xFF.
    while (rx_data == ADC_CONV_INCOMPLETE)
    {
        const bool is_status_ok = Io_SharedSpi_TransmitAndReceive(
            ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES, &rx_data, PLADC_RX_SIZE);

        if (!is_status_ok || (num_attempts >= MAX_NUM_ADC_COMPLETE_CHECKS))
        {
            return false;
        }

        num_attempts++;
    }

    return true;
}

bool Io_LTC6813Shared_WriteConfigurationRegisters(void)
{
    for (uint8_t curr_cfg_reg = 0U; curr_cfg_reg < NUM_OF_CFG_REGS; curr_cfg_reg++)
    {
        // Command used to write to a configuration register
        uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
            [CMD_WORD] = ltc6813_configs[curr_cfg_reg].cfg_reg_cmds, [CMD_PEC15] = 0U
        };

        // Array containing bytes to write to the configuration register
        uint8_t tx_cfg[NUM_OF_ACCUMULATOR_SEGMENTS][TOTAL_NUM_REG_GROUP_BYTES] = { 0U };

        // Prepare command to begin writing to the configuration
        // register
        Io_LTC6813Shared_PackCmdPec15(tx_cmd);

        // Prepare data to write to the configuration register
        Io_PrepareCfgRegBytes(tx_cfg, curr_cfg_reg);

        // Write to configuration registers
        Io_SharedSpi_SetNssLow(ltc6813_spi);
        if (Io_SharedSpi_TransmitWithoutNssToggle(ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES))
        {
            if (!Io_SharedSpi_TransmitWithoutNssToggle(ltc6813_spi, (uint8_t *)tx_cfg, NUM_REG_GROUP_RX_BYTES))
            {
                Io_SharedSpi_SetNssHigh(ltc6813_spi);
                return false;
            }
        }
        Io_SharedSpi_SetNssHigh(ltc6813_spi);
    }

    return true;
}

bool Io_LTC6813Shared_SetCfgRegsToDefaultSettings(void)
{
    // Send command on the isoSpi line to wake up the chip
    Io_LTC6813Shared_WriteConfigurationRegisters();

    // Write to the configuration registers
    return Io_LTC6813Shared_WriteConfigurationRegisters();
}

bool Io_LTC6813Shared_EnableDischarge(void)
{
    return Io_LTC6813Shared_SendCommand(UNMUTE);
}

bool Io_LTC6813Shared_DisableDischarge(void)
{
    return Io_LTC6813Shared_SendCommand(MUTE);
}
