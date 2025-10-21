#include "io/ltc6813.h"
#include "io/ltc6813/ltc6813_internal.h"

#define MAX_NUM_ADC_COMPLETE_CHECKS (10U)
#define ADC_CONV_INCOMPLETE (0xFF)

ExitCode io_ltc6813_pollAdcConversions(void)
{
    for (uint32_t num_attempts = 0U; num_attempts < MAX_NUM_ADC_COMPLETE_CHECKS; num_attempts++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(io_ltc6813_poll(PLADC, &rx_data, sizeof(rx_data)));

        if (rx_data != ADC_CONV_INCOMPLETE)
        {
            return EXIT_CODE_OK;
        }
    }

    return EXIT_CODE_TIMEOUT;
}

void io_ltc6813_wakeup(void)
{
    io_ltc6813_sendCommand(RDCFGA);
}
