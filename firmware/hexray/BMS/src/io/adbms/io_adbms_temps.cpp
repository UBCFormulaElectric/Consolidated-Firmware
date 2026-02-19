#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
constexpr uint8_t TEMPS_PER_GROUP = 3U;
constexpr uint8_t MAX_NUM_ATTEMPTS  = 10U;
constexpr uint8_t AUX_STATUS_READY = 0xFF;

std::expected<void, ErrorCode> clearCellTempReg()
{
    return sendCmd(CLRAUX);
}

std::expected<void, ErrorCode> startTempAdcConversion()
{
    RETURN_IF_ERR(clearCellTempReg());
    return sendCmd(ADAX_BASE);
}

std::expected<void, ErrorCode> pollTempAdcConversion() {
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(poll(PLCADC, { &rx_data, sizeof(rx_data) }));
        if (rx_data == AUX_STATUS_READY)
        {
            return {};
        }
    }
    return std::unexpected(ErrorCode::TIMEOUT);
}

void readCellTempReg(
    std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS> cell_temp_regs,
    std::expected<void, ErrorCode> comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    const std::expected<void, ErrorCode> poll_ok = pollTempAdcConversion();

    if (!poll_ok) {
        for (uint8_t i = 0U; i < NUM_SEGMENTS; i++) {
            for (uint8_t j = 0U; j < CELLS_PER_SEGMENT; j++) {
                comm_success[i][j] = poll_ok;
            }
        }
        return;
    }

    static constexpr std::array<uint16_t, 5> temp_read_cmds{ {RDAUXA, RDAUXB, RDAUXC, RDAUXD} };

    for (uint8_t reg_group = 0U; reg_group < NUM_VOLT_REG_GROUPS; reg_group++) {
        static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs;
        static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      group_success;
        readRegGroup(temp_read_cmds[reg_group], regs, group_success);

        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t temp_in_group = 0U; temp_in_group < TEMPS_PER_GROUP; temp_in_group++) {
                const auto temp_index = static_cast<uint8_t> (reg_group * TEMPS_PER_GROUP + temp_in_group);
                if (temp_index >= CELLS_PER_SEGMENT) {
                    continue;
                }
                comm_success[seg][temp_index] = group_success[seg];
                 if (!group_success[seg])
                {
                    continue;
                }
                const uint8_t byte_offset          = temp_in_group * 2U;
                cell_temp_regs[seg][temp_index] = regs[seg][byte_offset];
            
            }
        }
    }
}
} // namespace io::adbms
