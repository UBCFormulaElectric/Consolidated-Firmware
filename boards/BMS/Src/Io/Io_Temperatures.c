#include <stdint.h>
#include "Io_LTC6813.h"
#include "Io_Temperatures.h"
#include "Io_SharedSpi.h"
#include "configs/App_CellConfigs.h"
#include "configs/Io_LTC6813Configs.h"

static float internal_die_temperatures[NUM_OF_CELL_MONITOR_ICS];

// 0-100°C lookup table with 0.5°C resolution for a Vishay NTCALUG03A103G
// thermistor
//#define THERMISTOR_LUT_SIZE 201
// static const float THERM_LUT[THERMISTOR_LUT_SIZE] = {
//    32624.2f, 31804.3f, 31007.3f, 30232.8f, 29479.9f, 28747.9f, 28036.3f,
//    27344.5f, 26671.8f, 26017.6f, 25381.4f, 24762.6f, 24160.7f, 23575.3f,
//    23005.7f, 22451.6f, 21912.4f, 21387.8f, 20877.3f, 20380.5f, 19896.9f,
//    19426.2f, 18968.0f, 18522.0f, 18087.8f, 17664.9f, 17253.2f, 16852.3f,
//    16461.9f, 16081.6f, 15711.3f, 15350.5f, 14999.0f, 14656.6f, 14323.0f,
//    13998.0f, 13681.2f, 13372.6f, 13071.7f, 12778.5f, 12492.8f, 12214.2f,
//    11942.6f, 11677.8f, 11419.7f, 11168.0f, 10922.5f, 10683.2f, 10449.8f,
//    10222.0f, 10000.0f, 9783.4f,  9572.1f,  9365.9f,  9164.7f,  8968.5f,
//    8777.0f,  8590.1f,  8407.7f,  8229.7f,  8056.0f,  7886.4f,  7720.8f,
//    7559.2f,  7401.4f,  7247.4f,  7097.0f,  6950.1f,  6806.6f,  6666.6f,
//    6529.7f,  6396.1f,  6265.6f,  6138.1f,  6013.5f,  5891.8f,  5772.9f,
//    5656.7f,  5543.2f,  5432.3f,  5323.9f,  5217.9f,  5114.4f,  5013.2f,
//    4914.2f,  4817.5f,  4722.9f,  4630.5f,  4540.1f,  4451.7f,  4365.3f,
//    4280.8f,  4198.1f,  4117.3f,  4038.2f,  3960.9f,  3885.2f,  3811.2f,
//    3738.8f,  3668.0f,  3598.7f,  3530.9f,  3464.6f,  3399.7f,  3336.1f,
//    3273.9f,  3213.1f,  3153.5f,  3095.2f,  3038.1f,  2982.3f,  2927.6f,
//    2874.0f,  2821.6f,  2770.3f,  2720.0f,  2670.8f,  2622.6f,  2575.3f,
//    2529.1f,  2483.8f,  2439.5f,  2396.0f,  2353.4f,  2311.7f,  2270.9f,
//    2230.9f,  2191.6f,  2153.2f,  2115.6f,  2078.7f,  2042.5f,  2007.1f,
//    1972.3f,  1938.3f,  1904.9f,  1872.2f,  1840.1f,  1808.7f,  1777.9f,
//    1747.7f,  1718.0f,  1689.0f,  1660.5f,  1632.6f,  1605.2f,  1578.3f,
//    1552.0f,  1526.1f,  1500.8f,  1475.9f,  1451.5f,  1427.6f,  1404.2f,
//    1381.1f,  1358.5f,  1336.4f,  1314.6f,  1293.3f,  1272.4f,  1251.8f
//};

float *Io_Temperatures_GetDieTemperaturesDegC(void)
{
    return internal_die_temperatures;
}

ExitCode Io_Temperatures_ReadDieTemperaturesDegC(void)
{
    // The command used to read data from status register A.
    uint32_t RDSTATA = 0x0010;

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
        //
        //                                           (1°C * 100µV)
        // DIE_TEMP_DEG_C  = MEASURED_VOLTAGE_µV * ----------------- - 276°C
        //                                              7.6 mV

        internal_die_temperatures[current_ic] =
            (float)internal_die_temp * 100e-6f / 7.6e-3f - 276.0f;

        // The received PEC15 bytes are stored in the 6th and 7th byte.
        uint32_t received_pec15 =
            (uint32_t)(
                rx_internal_die_temp[6 + NUM_OF_RX_BYTES * current_ic] << 8) |
            (uint32_t)(rx_internal_die_temp[7 + NUM_OF_RX_BYTES * current_ic]);

        // Calculate the PEC15 using the first 6 bytes of data received from the
        // chip.
        uint32_t calculated_pec15 = Io_LTC6813_CalculatePec15(
            &rx_internal_die_temp[current_ic * NUM_OF_RX_BYTES], 6U);
        if (received_pec15 != calculated_pec15)
        {
            return EXIT_CODE_ERROR;
        }
    }

    return EXIT_CODE_OK;
}
