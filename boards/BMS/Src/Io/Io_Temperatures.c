#include <stdint.h>
#include "Io_LTC6813.h"
#include "Io_Temperatures.h"
#include "Io_SharedSpi.h"
#include "configs/App_CellConfigs.h"
#include "configs/Io_LTC6813Configs.h"

static float internal_die_temperatures[NUM_OF_CELL_MONITOR_ICS];

ExitCode Io_Temperatures_ReadDieTemperaturesDegC(void)
{
    // The command used to read data from status register A.
    const uint32_t RDSTATA = 0x0010;

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_StartInternalDeviceConversions())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions())

    uint8_t rx_internal_die_temp[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS] = {
        0
    };
    uint8_t tx_cmd[NUM_OF_CMD_BYTES];

    tx_cmd[0] = (uint8_t)(RDSTATA >> 8);
    tx_cmd[1] = (uint8_t)(RDSTATA);
    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)(tx_cmd_pec15);

    for (enum CellMonitorICs current_ic = CELL_MONITOR_IC_0;
         current_ic < NUM_OF_CELL_MONITOR_ICS; current_ic++)
    {
        if (Io_SharedSpi_TransmitAndReceive(
                Io_LTC6813_GetSpiInterface(), tx_cmd, NUM_OF_CMD_BYTES,
                rx_internal_die_temp,
                NUM_OF_CELL_MONITOR_ICS * NUM_OF_RX_BYTES) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        // The upper byte of the internal die temperature is stored in the
        // 3rd byte, while the lower byte is stored in the 2nd byte.
        const uint32_t internal_die_temp =
            (uint32_t)(rx_internal_die_temp[2 + NUM_OF_RX_BYTES * current_ic]) |
            (uint32_t)(
                (rx_internal_die_temp[3 + NUM_OF_RX_BYTES * current_ic] << 8));

        // Calculate the internal die temperature using the following equation:
        // DIE_TEMP = MEASURED_100UV * (1°C * 100µV / 7.6mV) - 276°C
        internal_die_temperatures[current_ic] =
            (float)internal_die_temp * 100e-6f / 7.6e-3f - 276.0f;

        // The received PEC15 bytes are stored in the 6th and 7th byte.
        const uint32_t received_pec15 =
            (uint32_t)(
                rx_internal_die_temp[6 + NUM_OF_RX_BYTES * current_ic] << 8) |
            (uint32_t)(rx_internal_die_temp[7 + NUM_OF_RX_BYTES * current_ic]);

        // Calculate the PEC15 using the first 6 bytes of data received from the
        // chip.
        const uint32_t calculated_pec15 = Io_LTC6813_CalculatePec15(
            &rx_internal_die_temp[current_ic * NUM_OF_RX_BYTES], 6U);
        if (received_pec15 != calculated_pec15)
        {
            return EXIT_CODE_ERROR;
        }
    }

    return EXIT_CODE_OK;
}

float *Io_Temperatures_GetDieTemperaturesDegC(void)
{
    return internal_die_temperatures;
}
