#include "io_adbms_internal.hpp"
#include <cstdint>

namespace io::adbms
{
constexpr uint8_t MAX_NUM_ATTEMPTS  = 10U;
constexpr uint8_t ADCV_STATUS_READY = 0xFF;
constexpr uint8_t CELLS_PER_GROUP   = 3U;

std::expected<void, ErrorCode> pollCellsAdcConversion()
{
    for (uint32_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(poll(PLCADC, { &rx_data, 1 }))
        if (rx_data == ADCV_STATUS_READY)
        {
            return {};
        }
    }
    return std::unexpected(ErrorCode::TIMEOUT);
}

std::expected<void, ErrorCode> clearCellVoltageReg()
{
    return sendCmd(CLRCELL);
}

std::expected<void, ErrorCode> startCellsAdcConversion()
{
    RETURN_IF_ERR(clearCellVoltageReg());
    return sendCmd(ADCV_BASE | RD | CONT);
}

void readVoltageRegisters(
    uint16_t                       cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    std::expected<void, ErrorCode> comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    if (const std::expected<void, ErrorCode> poll_ok = pollCellsAdcConversion(); not poll_ok)
    {
        for (uint8_t i = 0U; i < NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0U; j < CELLS_PER_SEGMENT; j++)
            {
                comm_success[i][j] = poll_ok;
            }
        }

        return;
    }

    static constexpr std::array<uint16_t, 5> voltage_read_cmds{ { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE } };
    for (uint8_t reg_group = 0U; reg_group < NUM_VOLT_REG_GROUPS; reg_group++)
    {
        static std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs;
        static std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      group_success;
        readRegGroup(voltage_read_cmds[reg_group], regs, group_success);

        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (uint8_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                const auto cell_index = static_cast<uint8_t>(reg_group * CELLS_PER_GROUP + cell_in_group);
                if (cell_index >= CELLS_PER_SEGMENT)
                {
                    continue;
                }

                comm_success[seg][cell_index] = group_success[seg];
                if (not group_success[seg])
                {
                    continue;
                }

                const uint8_t byte_offset          = cell_in_group * 2U;
                cell_voltage_regs[seg][cell_index] = regs[seg][byte_offset];
            }
        }
    }
}
} // namespace io::adbms
