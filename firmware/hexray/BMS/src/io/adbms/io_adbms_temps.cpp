#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

using namespace std;

static constexpr uint8_t MAX_NUM_ATTEMPTS = 10U;
static constexpr uint8_t GPIOS_PER_GROUP  = 3U;

static const array<uint16_t, io::adbms::NUM_TEMP_REG_GROUPS>              reg_groups{ {
    io::adbms::RDAUXA,
    io::adbms::RDAUXB,
    io::adbms::RDAUXC,
    io::adbms::RDAUXD,
} };

namespace io::adbms
{
expected<void, ErrorCode> clearCellTempReg()
{
    return sendCmd(CLRAUX);
}

expected<void, ErrorCode> startTempAdcConversion()
{
    return sendCmd(ADAX_BASE);
}

expected<void, ErrorCode> pollTempAdcConversion()
{
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
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

void readCellTempReg(
    array<array<uint16_t, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS>                  &cell_temp_regs,
    array<array<expected<void, ErrorCode>, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success)
{
    const expected<void, ErrorCode> poll_ok = pollTempAdcConversion();

    if (!poll_ok)
    {
        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            comm_success[seg].fill(poll_ok);
        }
        return;
    }

    for (size_t group = 0U; group < NUM_TEMP_REG_GROUPS; group++)
    {
        readRegGroup(reg_groups[group], shared_reg_group, shared_reg_group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!shared_reg_group_success[seg])
            {
                cell_temp_regs[seg].fill(0U);
                comm_success[seg].fill(shared_reg_group_success[seg]);
                continue;
            }

            for (size_t gpio_in_group = 0U; gpio_in_group < GPIOS_PER_GROUP; gpio_in_group++)
            {
                const size_t gpio = group * GPIOS_PER_GROUP + gpio_in_group;
                if (gpio < THERM_GPIOS_PER_SEGMENT)
                {
                    const uint16_t low         = shared_reg_group[seg][gpio_in_group * 2U];
                    const uint16_t high        = shared_reg_group[seg][gpio_in_group * 2U + 1U];
                    const uint16_t temperature = static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8U);

                    if (temperature == 0xFFFF || temperature == 0x8000)
                    {
                        cell_temp_regs[seg][gpio] = 0U;
                        comm_success[seg][gpio]   = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }

                    cell_temp_regs[seg][gpio] = temperature;
                    comm_success[seg][gpio]   = {};
                }
            }
        }
    }
}
} // namespace io::adbms
