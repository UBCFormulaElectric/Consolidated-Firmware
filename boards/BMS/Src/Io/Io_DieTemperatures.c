#include <stdint.h>
#include "Io_DieTemperatures.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/Io_LTC6813Configs.h"

static float internal_die_temp[NUM_OF_CELL_MONITOR_CHIPS];

ExitCode Io_DieTemperatures_ReadTemp(void)
{
    // The command used to start internal device conversions.
    const uint16_t ADSTAT = (0x468 + (MD << 7) + CHST);

    RETURN_CODE_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState());
    RETURN_CODE_IF_EXIT_NOT_OK(Io_LTC6813_SendCommand(ADSTAT));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions());

    uint8_t rx_internal_die_temp[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_CHIPS];

    // The command used to read data from status register A.
    const uint16_t RDSTATA = 0x0010;
    uint8_t        tx_cmd[NUM_OF_CMD_BYTES];
    tx_cmd[0] = (uint8_t)(RDSTATA >> 8);
    tx_cmd[1] = (uint8_t)(RDSTATA);
    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)(tx_cmd_pec15);

    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        if (Io_SharedSpi_TransmitAndReceive(
                Io_LTC6813_GetSpiInterface(), tx_cmd, NUM_OF_CMD_BYTES,
                rx_internal_die_temp,
                NUM_OF_CELL_MONITOR_CHIPS * NUM_OF_RX_BYTES) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        // The upper byte of the internal die temperature is stored in the
        // 3rd byte, while the lower byte is stored in the 2nd byte.
        const uint16_t _internal_die_temp = (uint16_t)(
            (rx_internal_die_temp[2 + NUM_OF_RX_BYTES * current_chip]) |
            (rx_internal_die_temp[3 + NUM_OF_RX_BYTES * current_chip] << 8));

        // Calculate the internal die temperature using the following equation:
        //
        //                                           (1°C * 100µV)
        // DIE_TEMP_DEG_C  = MEASURED_VOLTAGE_µV * ----------------- - 276°C
        //                                              7.6 mV

        internal_die_temp[current_chip] =
            (float)_internal_die_temp * 100e-6f / 7.6e-3f - 276.0f;

        // The received PEC15 bytes are stored in the 6th and 7th byte.
        const uint16_t received_pec15 = (uint16_t)(
            (rx_internal_die_temp[6 + NUM_OF_RX_BYTES * current_chip] << 8) |
            (rx_internal_die_temp[7 + NUM_OF_RX_BYTES * current_chip]));

        // Calculate the PEC15 using the first 6 bytes of data received from the
        // chip.
        const uint16_t calculated_pec15 = Io_LTC6813_CalculatePec15(
            &rx_internal_die_temp[current_chip * NUM_OF_RX_BYTES], 6U);

        if (received_pec15 != calculated_pec15)
        {
            return EXIT_CODE_ERROR;
        }
    }

    return EXIT_CODE_OK;
}

float Io_DieTemperatures_GetSegment0DieTemp(void)
{
    return internal_die_temp[0];
}

float Io_DieTemperatures_GetSegment1DieTemp(void)
{
    return internal_die_temp[1];
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
    float max_die_temp = internal_die_temp[0];
    for (size_t current_chip = 1U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        float current_die_temp = internal_die_temp[current_chip];
        if (max_die_temp < current_die_temp)
        {
            max_die_temp = current_die_temp;
        }
    }

    return max_die_temp;
}
