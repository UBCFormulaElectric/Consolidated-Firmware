#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

using namespace std;

namespace
{
constexpr uint8_t                              MAX_NUM_ATTEMPTS = 10U;
constexpr uint8_t                              GPIOS_PER_GROUP  = 3U;
constexpr array<uint16_t, NUM_TEMP_REG_GROUPS> reg_groups{ {
    io::adbms::RDAUXA,
    io::adbms::RDAUXB,
    io::adbms::RDAUXC,
    io::adbms::RDAUXD,
} };
} // namespace

namespace io::adbms
{
expected<void, ErrorCode> clear::CellAuxReg()
{
    return sendCmd(CLRAUX);
}

expected<void, ErrorCode> startTempAdcConversion()
{
    return sendCmd(ADAX_BASE);
}

expected<void, ErrorCode> startSegAdcConversion()
{
    return sendCmd(ADAX_BASE | CH4 | CH2);
}

expected<void, ErrorCode> pollAuxAdcConversion()
{
    for (size_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint32_t rx_data;
        RETURN_IF_ERR(poll(PLAUX, { reinterpret_cast<uint8_t *>(&rx_data), sizeof(rx_data) }));
        if (rx_data == POLL_STATUS_READY)
        {
            return {};
        }
    }
    return unexpected(ErrorCode::TIMEOUT);
}

Therms<std::expected<uint16_t, ErrorCode>> readCellTempReg()
{
    Therms<std::expected<uint16_t, ErrorCode>> cell_temp_regs;

    if (const expected<void, ErrorCode> poll_ok = pollAuxAdcConversion(); !poll_ok)
    {
        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            cell_temp_regs[seg].fill(unexpected(poll_ok.error()));
        }
        return cell_temp_regs;
    }

    for (size_t group = 0U; group < NUM_TEMP_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!out[seg])
            {
                cell_temp_regs[seg].fill(unexpected(out[seg].error()));
                continue;
            }

            for (size_t gpio_in_group = 0U; gpio_in_group < GPIOS_PER_GROUP; gpio_in_group++)
            {
                if (const size_t gpio = group * GPIOS_PER_GROUP + gpio_in_group; gpio < THERM_GPIOS_PER_SEGMENT)
                {
                    const uint16_t low  = out[seg].value()[gpio_in_group * 2U];
                    const uint16_t high = out[seg].value()[gpio_in_group * 2U + 1U];
                    const auto     temperature =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (temperature == 0xFFFF || temperature == 0x8000)
                    {
                        cell_temp_regs[seg][gpio] = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }
                    cell_temp_regs[seg][gpio] = temperature;
                }
            }
        }
    }
    return cell_temp_regs;
}

Segments<std::expected<uint16_t, ErrorCode>> readSegVoltageReg()
{
    Segments<std::expected<uint16_t, ErrorCode>> segment_voltage_regs;
    if (const expected<void, ErrorCode> poll_ok = pollAuxAdcConversion(); !poll_ok)
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            segment_voltage_regs[seg] = unexpected(poll_ok.error());
        }
        return segment_voltage_regs;
    }

    const auto out = readRegGroup(RDAUXD);
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!out[seg])
        {
            segment_voltage_regs[seg] = std::unexpected(out[seg].error());
            continue;
        }

        const uint8_t low     = out[seg].value()[4U];
        const uint8_t high    = out[seg].value()[5U];
        const auto    voltage = static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

        if (voltage == 0xFFFF || voltage == 0x8000)
        {
            segment_voltage_regs[seg] = std::unexpected(ErrorCode::ERROR);
            continue;
        }
        segment_voltage_regs[seg] = voltage;
    }
    return segment_voltage_regs;
}
} // namespace io::adbms
