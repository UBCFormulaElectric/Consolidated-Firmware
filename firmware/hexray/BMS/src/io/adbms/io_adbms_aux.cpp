#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;

namespace
{
constexpr uint8_t                               MAX_NUM_ATTEMPTS     = 20U;
constexpr uint8_t                               POLL_RETRY_DELAY_MS  = 1U;
constexpr uint8_t                               GPIOS_PER_GROUP      = 3U;
constexpr uint8_t                               NUM_THERM_REG_GROUPS = 3U;
constexpr array<uint16_t, NUM_THERM_REG_GROUPS> reg_groups{ {
    io::adbms::RDAUXA,
    io::adbms::RDAUXB,
    io::adbms::RDAUXC,
} };
} // namespace

namespace io::adbms
{
result<void> clear::CellAuxReg()
{
    return sendCmd(CLRAUX);
}

result<void> startTempAdcConversion()
{
    return sendCmd(ADAX_BASE);
}

result<void> startSegAdcConversion()
{
    return sendCmd(ADAX_BASE | CH4 | CH2);
}

result<void> pollAuxAdcConversion()
{
    for (size_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint32_t rx_data;
        RETURN_IF_ERR(poll(PLAUX, { reinterpret_cast<uint8_t *>(&rx_data), sizeof(rx_data) }));
        if (rx_data == POLL_STATUS_READY)
        {
            return {};
        }
        io::time::delay(POLL_RETRY_DELAY_MS);
    }
    return unexpected(ErrorCode::TIMEOUT);
}

result<Therms<expected<uint16_t, ErrorCode>>> readCellTempReg()
{
    Therms<result<uint16_t>> cell_temp_regs{};

    if (const result<void> poll_ok = pollAuxAdcConversion(); !poll_ok)
    {
        return unexpected(poll_ok.error());
    }

    for (size_t group = 0U; group < NUM_THERM_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t gpio_in_group = 0U; gpio_in_group < GPIOS_PER_GROUP; gpio_in_group++)
            {
                if (const size_t gpio = group * GPIOS_PER_GROUP + gpio_in_group; gpio < THERM_GPIOS_PER_SEGMENT)
                {
                    if (!out[seg])
                    {
                        cell_temp_regs[seg][gpio] = unexpected(out[seg].error());
                        continue;
                    }
                    const uint16_t low  = out[seg].value()[gpio_in_group * 2U];
                    const uint16_t high = out[seg].value()[gpio_in_group * 2U + 1U];
                    const auto     temperature =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (temperature == 0xFFFF || temperature == 0x8000)
                    {
                        cell_temp_regs[seg][gpio] = std::unexpected(ErrorCode::INVALID_READING);
                        continue;
                    }
                    cell_temp_regs[seg][gpio] = temperature;
                }
            }
        }
    }
    return cell_temp_regs;
}

result<Segments<expected<uint16_t, ErrorCode>>> readSegVoltageReg()
{
    Segments<result<uint16_t>> segment_voltage_regs{};
    if (const result<void> poll_ok = pollAuxAdcConversion(); !poll_ok)
    {
        return unexpected(poll_ok.error());
    }

    const auto out = readRegGroup(RDAUXD);
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!out[seg])
        {
            segment_voltage_regs[seg] = unexpected(out[seg].error());
            continue;
        }

        const uint8_t low     = out[seg].value()[4U];
        const uint8_t high    = out[seg].value()[5U];
        const auto    voltage = static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

        if (voltage == 0xFFFF || voltage == 0x8000)
        {
            segment_voltage_regs[seg] = std::unexpected(ErrorCode::INVALID_READING);
            continue;
        }
        segment_voltage_regs[seg] = voltage;
    }
    return segment_voltage_regs;
}
} // namespace io::adbms
