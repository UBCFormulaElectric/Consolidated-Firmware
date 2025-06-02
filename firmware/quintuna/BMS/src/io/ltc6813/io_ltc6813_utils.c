#include "io_ltc6813.h"
#include "io_ltc6813_internal.h"
#include "io_time.h"

#define MAX_NUM_ADC_COMPLETE_CHECKS (10U)
#define ADC_CONV_COMPLETE (255U) // experimentally this is true?? it generally reads 127 if it is pending

ExitCode io_ltc6813_pollAdcConversions(void)
{
    for (uint32_t num_attempts = 0U; num_attempts < MAX_NUM_ADC_COMPLETE_CHECKS; num_attempts++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(io_ltc6813_poll(PLADC, &rx_data, sizeof(rx_data)));

        if (rx_data != ADC_CONV_COMPLETE)
        {
            return EXIT_CODE_OK;
        }

        io_time_delay(1);
    }

    return EXIT_CODE_TIMEOUT;
}
