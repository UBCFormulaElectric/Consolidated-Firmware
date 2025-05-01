#include "app_utils.h"
#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#define MAX_NUM_ADC_COMPLETE_CHECKS (10U)
#define PLADC (0x0714U)
#define ADC_CONV_COMPLETE (255U) // experimentally this is true?? it generally reads 127 if it is pending
ExitCode io_ltc6813_pollAdcConversions(void)
{
    // Prepare command to get the status of ADC conversions
    const ltc6813_tx tx_cmd = io_ltc6813_build_tx_cmd(PLADC);
    for (uint8_t num_attempts = 0U; num_attempts < MAX_NUM_ADC_COMPLETE_CHECKS; num_attempts++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(
            hw_spi_transmitThenReceive(&ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), &rx_data, sizeof(rx_data)));

        if (rx_data == ADC_CONV_COMPLETE)
        {
            return EXIT_CODE_OK;
        }
    }

    return EXIT_CODE_RETRY_FAILED;
}
