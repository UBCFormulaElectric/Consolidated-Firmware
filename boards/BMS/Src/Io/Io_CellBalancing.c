#include <string.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

// clang-format off
#define DEFAULT_CFGRA_CONFIG                                                       \
    {                                                                              \
        [REG_GROUP_0] = (uint8_t)(ENABLE_ALL_CFGRA_GPIO | REFON | DTEN | ADCOPT),  \
        [REG_GROUP_1] = (uint8_t)VUV,                                              \
        [REG_GROUP_2] = (uint8_t)((VOV & 0xF) << 4) + (VUV >> 8),                  \
        [REG_GROUP_3] = (uint8_t)(VOV >> 4), [REG_GROUP_4] = 0x0U,                 \
        [REG_GROUP_5] = 0x0U,                                                      \
        [REG_GROUP_PEC0] = 0U,                                                     \
        [REG_GROUP_PEC1] = 0U,                                                     \
    }

#define DEFAULT_CFGRB_CONFIG                   \
    {                                          \
        [REG_GROUP_0] = ENABLE_ALL_CFGRB_GPIO, \
        [REG_GROUP_1] = 0U,                    \
        [REG_GROUP_2] = 0U,                    \
        [REG_GROUP_3] = 0U,                    \
        [REG_GROUP_4] = 0U,                    \
        [REG_GROUP_5] = 0U,                    \
        [REG_GROUP_PEC0] = 0U,                 \
        [REG_GROUP_PEC1] = 0U,                 \
    }

// Macros for setting DCC bits
#define SET_ALL_DCC_BITS (0xFFFFU)
#define SET_MIN_CELL_DCC_BIT(index) ((uint16_t)(1U << (index)))
#define SET_CFGRA4_DCC_BITS(dcc_bits) (0xFFU & (uint8_t)(dcc_bits))
#define SET_CFGRA5_DCC_BITS(dcc_bits) (0x0FU & (uint8_t)((dcc_bits) >> 8U))
#define SET_CFGRB0_DCC_BITS(dcc_bits) (0xF0U & (uint8_t)((dcc_bits) >> 8U))
// clang-format on

extern struct SharedSpi *ltc6813_spi;

#include "Io_CellVoltages.h"

static void Io_CellBalancing_PrepareConfigRegBytes(
    uint8_t tx_cfg[NUM_OF_ACCUMULATOR_SEGMENTS][NUM_REG_GROUP_BYTES],
    uint8_t curr_cfg_reg)
{
    const uint8_t default_cfg_regs[NUM_OF_CFG_REGISTERS]
                                  [NUM_REG_GROUP_BYTES] = {
                                      [CONFIG_REG_A] = DEFAULT_CFGRA_CONFIG,
                                      [CONFIG_REG_B] = DEFAULT_CFGRB_CONFIG,
                                  };

    uint8_t        min_cell_segment = 0U;
    uint8_t        min_cell_index   = 0U;
    const uint16_t min_cell_dcc_bits =
        (uint16_t)(~(SET_MIN_CELL_DCC_BIT(min_cell_index)));
    //Io_CellVoltages_GetMinCellLocation(&min_cell_segment, &min_cell_index);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        // Copy default configurations into tx_cfg for each segment
        memcpy(
            &tx_cfg[curr_segment], default_cfg_regs[curr_cfg_reg],
            NUM_REG_GROUP_BYTES);

        // Get dcc bits to write for the current segment
        uint16_t dcc_bits = (curr_segment == (NUM_OF_ACCUMULATOR_SEGMENTS -
                                              min_cell_segment - 1))
                                ? min_cell_dcc_bits
                                : SET_ALL_DCC_BITS;

        if (curr_cfg_reg == CONFIG_REG_A)
        {
            tx_cfg[curr_segment][REG_GROUP_4] |= SET_CFGRA4_DCC_BITS(dcc_bits);
            tx_cfg[curr_segment][REG_GROUP_5] |= SET_CFGRA5_DCC_BITS(dcc_bits);
        }
        else
        {
            tx_cfg[curr_segment][REG_GROUP_0] |= SET_CFGRB0_DCC_BITS(dcc_bits);
        }

        Io_LTC6813_PackPec15(tx_cfg[curr_segment], NUM_OF_REGS_IN_GROUP);
    }
}

void Io_CellBalancing_ConfigureDccBits(void)
{
    const uint16_t cfg_reg_cmds[NUM_OF_CFG_REGISTERS] = {
        [CONFIG_REG_A] = WRCFGA,
        [CONFIG_REG_B] = WRCFGB,
    };

    for (uint8_t curr_cfg_reg = 0U; curr_cfg_reg < NUM_OF_CFG_REGISTERS;
         curr_cfg_reg++)
    {
        // Command used to write to a configuration register
        // Command containing bytes to write to the configuration register
        uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES] = { 0U };
        uint8_t tx_cfg[NUM_OF_ACCUMULATOR_SEGMENTS][NUM_REG_GROUP_BYTES] = {
            { 0U }
        };

        // Prepare command to start writing to the configuration register
        Io_LTC6813_PrepareCmd(tx_cmd, cfg_reg_cmds[curr_cfg_reg]);

        // Prepare configuration register bytes to write to the configuration register
        Io_CellBalancing_PrepareConfigRegBytes(tx_cfg, curr_cfg_reg);

        // Write to configuration registers
        Io_SharedSpi_SetNssLow(ltc6813_spi);
        Io_SharedSpi_TransmitWithoutNssToggle(
            ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES);
        Io_SharedSpi_TransmitWithoutNssToggle(
            ltc6813_spi, &tx_cfg[0][0], TOTAL_NUM_OF_REG_BYTES);
        Io_SharedSpi_SetNssHigh(ltc6813_spi);
    }
}
