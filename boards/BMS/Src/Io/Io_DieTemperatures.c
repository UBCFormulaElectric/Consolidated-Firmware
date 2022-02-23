#include <stdint.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

// Command to read from status register group A
#define RDSTATA (0x0010U)

// Command used to start internal measurements for the LTC6813
#define MD (1U)
#define CHST (0U)
#define ADSTAT (0x0468U + (MD << 7) + CHST)

#define DIE_TEMP_MV_TO_DEGC (1.0f / 7.6e-3f)
#define DIE_TEMP_TO_DEGC_SCALE (V_PER_100UV * DIE_TEMP_MV_TO_DEGC)
#define DIE_TEMP_OFFSET_DEGC (276.0f)

struct DieTemperature
{
    uint8_t  segment;
    uint8_t  ic;
    uint16_t temp;
};

extern struct SharedSpi *ltc6813_spi;

static struct DieTemperature max_die_temp                          = { 0U };
static uint16_t              die_temp[NUM_OF_ACCUMULATOR_SEGMENTS] = { 0.0f };

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

bool Io_DieTemperatures_StartDieTempConversion(void)
{
    return Io_LTC6813_SendCommand(ADSTAT);
}

bool Io_DieTemperatures_ReadTemp(void)
{
    bool    status                              = true;
    uint8_t rx_die_temp[TOTAL_NUM_OF_REG_BYTES] = { 0U };
    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES]         = { 0U };

    // The command used to read data from status register A.
    Io_LTC6813_PrepareCmd(tx_cmd, RDSTATA);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        const uint32_t die_temp_index =
            (uint32_t)(curr_segment * NUM_REG_GROUP_BYTES);

        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES, rx_die_temp,
                TOTAL_NUM_OF_REG_BYTES))
        {
            // Store the internal die temperature. The upper byte of the
            // internal die temperature is stored in the 3rd byte, while the
            // lower byte is stored in the 2nd byte.
            const uint16_t raw_die_temp = (uint16_t)(
                (rx_die_temp[REG_GROUP_2 + die_temp_index]) |
                (rx_die_temp[REG_GROUP_3 + die_temp_index] << 8));
            die_temp[curr_segment] = raw_die_temp;

            // TODO: fix indexing
            // The received PEC15 bytes are stored in the 6th and 7th byte.
            const uint16_t received_pec15 = (uint16_t)(
                (rx_die_temp[REG_GROUP_PEC0 + die_temp_index] << 8) |
                (rx_die_temp[REG_GROUP_PEC1 + die_temp_index]));

            // Calculate the PEC15 using the first 6 bytes of data received from
            // the chip.
            const uint16_t calculated_pec15 =
                Io_LTC6813_CalculateRegGroupPec15(&rx_die_temp[die_temp_index]);

            if (received_pec15 != calculated_pec15)
            {
                status = false;
                break;
            }
        }
    }

    return status;
}

float Io_DieTemperatures_GetSegmentDieTemp(AccumulatorSegments_E segment)
{
    return Io_DieTemperatures_ConvertToDegC(die_temp[segment]);
}

float Io_DieTemperatures_GetMaxDieTemp(void)
{
    return Io_DieTemperatures_ConvertToDegC(max_die_temp.temp);
}

void Io_DieTemperatures_GetMaxDieTempLocation(uint8_t *segment, uint8_t *ic)
{
    *segment = max_die_temp.segment;
    *ic      = max_die_temp.ic;
}
