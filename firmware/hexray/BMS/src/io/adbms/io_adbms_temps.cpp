#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

using namespace std;

static constexpr uint8_t MAX_NUM_ATTEMPTS  = 10U;
static constexpr uint8_t THERMISTORS_PER_GROUP = 3U;

static array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> regs;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS> group_success;
static const array<uint16_t, io::adbms::NUM_TEMP_REG_GROUPS> reg_groups{ {
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

expected<void, ErrorCode> pollTempAdcConversion() {
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint32_t rx_data;
        RETURN_IF_ERR(poll(
            PLCADC,
            { reinterpret_cast<uint8_t *>(&rx_data), sizeof(rx_data) }));
        if (rx_data == POLL_STATUS_READY)
        {
            return {};
        }
    }
    return unexpected(ErrorCode::TIMEOUT);
}

void readCellTempReg(
    array<array<uint16_t, THERMISTORS_PER_SEGMENT>, NUM_SEGMENTS> &cell_temp_regs,
    array<array<expected<void, ErrorCode>, THERMISTORS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success)
{
    const expected<void, ErrorCode> poll_ok = pollTempAdcConversion();

    if (!poll_ok) {
        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++) {
                comm_success[seg][therm] = poll_ok;
            }
        }
        return;
    }

    for (size_t group = 0U; group < NUM_TEMP_REG_GROUPS; group++) {
        readRegGroup(reg_groups[group], regs, group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++) {
            if (!group_success[seg]) {
                comm_success[seg].fill(group_success[seg]);
                continue;
            }

            for (size_t therm_in_group = 0U; therm_in_group < THERMISTORS_PER_SEGMENT; therm_in_group++) {
                const size_t therm = group * THERMISTORS_PER_GROUP + therm_in_group;
                if (therm < THERMISTORS_PER_SEGMENT) {
                    const uint16_t low  = regs[seg][therm_in_group * 2U];
                    const uint16_t high = regs[seg][therm_in_group * 2U + 1U];
                    const uint16_t temperature = static_cast<uint16_t>(low | (high << 8U));                    
                    cell_temp_regs[seg][therm] = temperature;
                    comm_success[seg][therm] = {};
                }
            }
        }
    }
}
} // namespace io::adbms
