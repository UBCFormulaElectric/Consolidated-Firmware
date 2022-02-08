#include <stdint.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

#define DIE_TEMP_MV_TO_DEGC (1.0f / 7.6e-3f)
#define DIE_TEMP_TO_DEGC_SCALE (V_PER_100UV * DIE_TEMP_MV_TO_DEGC)
#define DIE_TEMP_OFFSET_DEGC (276.0f)

#define RDSTATA (0x0010U)

extern struct SharedSpi *ltc6813_spi;

extern float internal_die_temp[NUM_OF_ACCUMULATOR_SEGMENTS];
float        internal_die_temp[NUM_OF_ACCUMULATOR_SEGMENTS] = { 0.0f };

static inline float Io_DieTemperatures_ConvertToDegC(uint16_t raw_die_temp_v)
{
    // Calculate the internal die temperature using the following
    // equation:
    //
    //                                           (1°C * 100µV)
    // DIE_TEMP_DEG_C  = MEASURED_VOLTAGE_µV * ----------------- - 276°C
    //                                              7.6 mV
    return (float)(raw_die_temp_v)*DIE_TEMP_TO_DEGC_SCALE -
           DIE_TEMP_OFFSET_DEGC;
}

bool Io_DieTemperatures_ReadTemp(void)
{
    bool    status                                       = true;
    uint8_t rx_internal_die_temp[TOTAL_NUM_OF_REG_BYTES] = { 0U };
    uint8_t tx_cmd[NUM_TX_CMD_BYTES]                     = { 0U };

    // The command used to read data from status register A.
    Io_LTC6813_PackCmd(tx_cmd, RDSTATA);
    Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        const uint32_t die_temp_index =
            (uint32_t)(curr_segment * NUM_REG_GROUP_BYTES);

        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES, rx_internal_die_temp,
                TOTAL_NUM_OF_REG_BYTES))
        {
            // Store the internal die temperature. The upper byte of the
            // internal die temperature is stored in the 3rd byte, while the
            // lower byte is stored in the 2nd byte.
            const uint16_t raw_die_temp = (uint16_t)(
                (rx_internal_die_temp[LTC6813_REG_2 + die_temp_index]) |
                (rx_internal_die_temp[LTC6813_REG_3 + die_temp_index] << 8));
            internal_die_temp[curr_segment] =
                Io_DieTemperatures_ConvertToDegC(raw_die_temp);

            // The received PEC15 bytes are stored in the 6th and 7th byte.
            const uint16_t received_pec15 = (uint16_t)(
                (rx_internal_die_temp[6 + die_temp_index] << 8) |
                (rx_internal_die_temp[7 + die_temp_index]));

            // Calculate the PEC15 using the first 6 bytes of data received from
            // the chip.
            const uint16_t calculated_pec15 = Io_LTC6813_CalculatePec15(
                &rx_internal_die_temp[die_temp_index], NUM_OF_REGS_IN_GROUP);

            if (received_pec15 != calculated_pec15)
            {
                status = false;
                break;
            }
        }
    }

    return status;
}

float Io_DieTemperatures_GetSegment0DieTemp(void)
{
    return internal_die_temp[0];
}

float Io_DieTemperatures_GetSegment1DieTemp(void)
{
    // return internal_die_temp[1];
    return 0.0f;
}

float Io_DieTemperatures_GetSegment2DieTemp(void)
{
    return 0.0f;
}

float Io_DieTemperatures_GetSegment3DieTemp(void)
{
    return 0.0f;
}

float Io_DieTemperatures_GetSegment4DieTemp(void)
{
    return 0.0f;
}

float Io_DieTemperatures_GetSegment5DieTemp(void)
{
    return 0.0f;
}

float Io_DieTemperatures_GetMaxDieTemp(void)
{
    return 0.0f;
}
