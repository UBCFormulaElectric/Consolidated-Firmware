#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include <cstdint>

using namespace std;
namespace
{
constexpr uint8_t MAX_NUM_ATTEMPTS = 10U;
constexpr uint8_t CELLS_PER_GROUP  = 3U;
} // namespace

namespace io::adbms
{
expected<void, ErrorCode> clear::CellVoltageReg()
{
    return sendCmd(CLRCELL);
}

expected<void, ErrorCode> clear::FilteredCellVoltageReg()
{
    return sendCmd(CLRFC);
}

expected<void, ErrorCode> startCellsAdcConversion()
{
    return sendCmd(ADCV_BASE | RD);
}

expected<void, ErrorCode> pollCellsAdcConversion()
{
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint32_t rx_data;
        RETURN_IF_ERR(poll(PLCADC, { reinterpret_cast<uint8_t *>(&rx_data), sizeof(rx_data) }));
        if (rx_data == POLL_STATUS_READY)
        {
            return {};
        }
    }
    return unexpected(ErrorCode::TIMEOUT);
}

Cells<expected<uint16_t, ErrorCode>> readCellVoltageReg()
{
    static constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> cell_voltage_reg_groups{ { RDCVA, RDCVB, RDCVC, RDCVD,
                                                                                     RDCVE } };

    Cells<expected<uint16_t, ErrorCode>> cell_voltage_regs;
    if (const auto poll_ok = pollCellsAdcConversion(); !poll_ok)
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            cell_voltage_regs[seg].fill(unexpected(poll_ok.error()));
        }
        return cell_voltage_regs;
    }

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(cell_voltage_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!out[seg])
            {
                cell_voltage_regs[seg].fill(std::unexpected(out[seg].error()));
                continue;
            }
            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                if (const size_t cell = group * CELLS_PER_GROUP + cell_in_group; cell < CELLS_PER_SEGMENT)
                {
                    const uint8_t low  = out[seg].value()[cell_in_group * 2U];
                    const uint8_t high = out[seg].value()[cell_in_group * 2U + 1U];
                    const auto    voltage =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);
                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }
                    cell_voltage_regs[seg][cell] = voltage;
                }
            }
        }
    }
    return cell_voltage_regs;
}

Cells<expected<uint16_t, ErrorCode>> readFilteredCellVoltageReg()
{
    static constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> filtered_reg_groups{ { RDFCA, RDFCB, RDFCC, RDFCD, RDFCE } };

    Cells<expected<uint16_t, ErrorCode>> filtered_cell_voltage_regs;
    if (const auto poll_ok = pollCellsAdcConversion(); !poll_ok)
    {
        // TODO if this fails, then it should
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            filtered_cell_voltage_regs[seg].fill(std::unexpected(poll_ok.error()));
        }
        return filtered_cell_voltage_regs;
    }

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(filtered_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!out[seg])
            {
                filtered_cell_voltage_regs[seg].fill(std::unexpected(out[seg].error()));
                continue;
            }

            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                if (const size_t cell = group * CELLS_PER_GROUP + cell_in_group; cell < CELLS_PER_SEGMENT)
                {
                    const uint8_t low  = out[seg].value()[cell_in_group * 2U];
                    const uint8_t high = out[seg].value()[cell_in_group * 2U + 1U];
                    const auto    voltage =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        filtered_cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }
                    filtered_cell_voltage_regs[seg][cell] = voltage;
                }
            }
        }
    }
    return filtered_cell_voltage_regs;
}

} // namespace io::adbms
