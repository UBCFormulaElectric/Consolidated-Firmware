#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "io_time.hpp"
#include <cstdint>

using namespace std;
namespace
{
constexpr uint8_t MAX_NUM_ATTEMPTS    = 20U;
constexpr uint8_t POLL_RETRY_DELAY_MS = 1U;
constexpr uint8_t CELLS_PER_GROUP     = 3U;
} // namespace

namespace io::adbms
{
result<void> clear::CellVoltageReg()
{
    return sendCmd(CLRCELL);
}

result<void> clear::FilteredCellVoltageReg()
{
    return sendCmd(CLRFC);
}

result<void> startCellsAdcConversion()
{
    return sendCmd(ADCV_BASE);
}

result<void> pollCellsAdcConversion()
{
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        const auto rx_res = poll(PLCADC);
        RETURN_IF_ERR_SILENT(rx_res);
        if (const std::bitset<32> rx_data = rx_res.value(); rx_data.to_ulong() == POLL_STATUS_READY)
        {
            return {};
        }
        io::time::delay(POLL_RETRY_DELAY_MS);
    }
    return unexpected(ErrorCode::TIMEOUT);
}

result<Cells<expected<uint16_t, ErrorCode>>> readCellVoltageReg()
{
    static constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> cell_voltage_reg_groups{ { RDCVA, RDCVB, RDCVC, RDCVD,
                                                                                     RDCVE } };

    Cells<result<uint16_t>> cell_voltage_regs{};
    if (const auto poll_ok = pollCellsAdcConversion(); !poll_ok)
    {
        return unexpected(poll_ok.error());
    }

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(cell_voltage_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                if (const size_t cell = group * CELLS_PER_GROUP + cell_in_group; cell < CELLS_PER_SEGMENT)
                {
                    if (!out[seg])
                    {
                        cell_voltage_regs[seg][cell] = unexpected(out[seg].error());
                        continue;
                    }
                    const uint8_t low  = out[seg].value()[cell_in_group * 2U];
                    const uint8_t high = out[seg].value()[cell_in_group * 2U + 1U];
                    const auto    voltage =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);
                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
                        continue;
                    }
                    cell_voltage_regs[seg][cell] = voltage;
                }
            }
        }
    }
    return cell_voltage_regs;
}

result<Cells<expected<uint16_t, ErrorCode>>> readFilteredCellVoltageReg()
{
    static constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> filtered_reg_groups{ { RDFCA, RDFCB, RDFCC, RDFCD, RDFCE } };

    Cells<result<uint16_t>> filtered_cell_voltage_regs{};
    if (const auto poll_ok = pollCellsAdcConversion(); !poll_ok)
    {
        return unexpected(poll_ok.error());
    }

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(filtered_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                if (const size_t cell = group * CELLS_PER_GROUP + cell_in_group; cell < CELLS_PER_SEGMENT)
                {
                    if (!out[seg])
                    {
                        filtered_cell_voltage_regs[seg][cell] = unexpected(out[seg].error());
                        continue;
                    }
                    const uint8_t low  = out[seg].value()[cell_in_group * 2U];
                    const uint8_t high = out[seg].value()[cell_in_group * 2U + 1U];
                    const auto    voltage =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        filtered_cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
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
