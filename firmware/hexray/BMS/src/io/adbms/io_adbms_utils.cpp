#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
constexpr uint16_t MAX_NUM_ADC_COMPLETE_CHECKS = 10;
constexpr uint8_t  ADC_CONV_INCOMPLETE         = 0xFF;

std::expected<void, ErrorCode> pollAdcConversions()
{
    for (uint32_t num_attempts = 0U; num_attempts < MAX_NUM_ADC_COMPLETE_CHECKS; num_attempts++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(poll(PLADC, { &rx_data, sizeof(rx_data) }));

        if (rx_data != ADC_CONV_INCOMPLETE)
        {
            return {};
        }
    }
    return std::unexpected(ErrorCode::TIMEOUT);
}

std::expected<void, ErrorCode> wakeup()
{
    RETURN_IF_ERR(sendCmd(RDCFGA));
    return {};
}

} // namespace io::adbms
