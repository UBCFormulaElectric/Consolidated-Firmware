#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "io_time.hpp"
#include "util_retry.hpp"
#include <cstdint>

using namespace std;
namespace
{
constexpr uint8_t CELLS_PER_GROUP     = 3U;
constexpr uint8_t NUM_VOLT_REG_GROUPS = 5;
} // namespace

namespace io::adbms
{
result<void> clear::cell()
{
    return sendCmd(CLRCELL);
}

result<void> clear::filteredCell()
{
    return sendCmd(CLRFC);
}

result<void> command::startCellsAdc()
{
    return sendCmd(ADCV_BASE);
}

result<void> command::owcCells(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EVEN_CHANNELS) ? (ADSV_BASE | OW0) : (ADSV_BASE | OW1);
    return sendCmd(cmd);
}

result<void> command::pollCellsAdc()
{
    uint32_t attempt = 0;
    return util::retry(
        [&attempt]() -> result<void>
        {
            ++attempt;
            LOG_INFO(
                "pollCellAdc retry attempt %lu/%lu", static_cast<unsigned long>(attempt),
                static_cast<unsigned long>(POLL_RETRIES));
            const auto rx_res = poll(PLCADC);
            if (!rx_res)
                return unexpected(rx_res.error());
            const auto POLL_RES = rx_res.value().to_ulong();
            if (POLL_RES == POLL_STATUS_READY)
                return {};
            io::time::delay(POLL_RETRY_DELAY_MS);
            return unexpected(ErrorCode::BUSY);
        },
        20);
}

result<void> command::pollSecondaryCellsAdc()
{
    return util::retry(
        []() -> result<void>
        {
            const auto rx_res = poll(PLSADC);
            RETURN_IF_ERR_SILENT(rx_res);
            if (rx_res.value().to_ulong() == POLL_STATUS_READY)
                return {};
            io::time::delay(POLL_RETRY_DELAY_MS);
            return unexpected(ErrorCode::POLL_INVALID);
        },
        POLL_RETRIES);
}

Cells<result<int16_t>> read::cellVoltage()
{
    Cells<result<int16_t>>                         cell_voltage_regs{};
    constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> cell_voltage_reg_groups{ { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE } };

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const Segments<result<RegBuffer>> seg_reg = readRegGroup(cell_voltage_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                const size_t cell = group * CELLS_PER_GROUP + cell_in_group;
                if (cell >= CELLS_PER_SEGMENT)
                {
                    continue;
                }
                if (!seg_reg[seg])
                {
                    cell_voltage_regs[seg][cell] = unexpected(seg_reg[seg].error());
                    continue;
                }
                const uint8_t low  = seg_reg[seg].value()[cell_in_group * 2U];
                const uint8_t high = seg_reg[seg].value()[cell_in_group * 2U + 1U];
                const auto    voltage_reg_val =
                    static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);
                if (voltage_reg_val == DEFAULT_REGISTER_VALUE)
                {
                    cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
                    continue;
                }
                cell_voltage_regs[seg][cell] = voltage_reg_val;
            }
        }
    }
    return cell_voltage_regs;
}

// Cells<result<uint16_t>> read::filteredCellVoltage()
// {
//     Cells<result<uint16_t>>              filtered_cell_voltage_regs{};
//     array<uint16_t, NUM_VOLT_REG_GROUPS> filtered_reg_groups{ { RDFCA, RDFCB, RDFCC, RDFCD, RDFCE } };
//
//     for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
//     {
//         const Segments<result<RegBuffer>> out = readRegGroup(filtered_reg_groups[group]);
//
//         for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
//         {
//             for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
//             {
//                 const size_t cell = group * CELLS_PER_GROUP + cell_in_group;
//                 if (cell >= CELLS_PER_SEGMENT)
//                 {
//                     continue;
//                 }
//                 if (!out[seg])
//                 {
//                     filtered_cell_voltage_regs[seg][cell] = unexpected(out[seg].error());
//                     continue;
//                 }
//                 const uint8_t low  = out[seg].value()[cell_in_group * 2U];
//                 const uint8_t high = out[seg].value()[cell_in_group * 2U + 1U];
//                 const auto    voltage =
//                     static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);
//                 if (voltage == DEFAULT_REGISTER_VALUE)
//                 {
//                     filtered_cell_voltage_regs[seg][cell] = std::unexpected(ErrorCode::INVALID_READING);
//                     continue;
//                 }
//                 filtered_cell_voltage_regs[seg][cell] = voltage;
//             }
//         }
//     }
//     return filtered_cell_voltage_regs;
// }

Cells<result<int16_t>> read::secondaryCellVoltage()
{
    Cells<result<int16_t>>                         cell_voltage_regs{};
    constexpr array<uint16_t, NUM_VOLT_REG_GROUPS> cell_voltage_reg_groups{ { RDSVA, RDSVB, RDSVC, RDSVD, RDSVE } };

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        const Segments<result<RegBuffer>> seg_reg = readRegGroup(cell_voltage_reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                const size_t cell = group * CELLS_PER_GROUP + cell_in_group;
                if (cell >= CELLS_PER_SEGMENT)
                {
                    continue;
                }
                if (!seg_reg[seg])
                {
                    cell_voltage_regs[seg][cell] = unexpected(seg_reg[seg].error());
                    continue;
                }
                const uint8_t low  = seg_reg[seg].value()[cell_in_group * 2U];
                const uint8_t high = seg_reg[seg].value()[cell_in_group * 2U + 1U];
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
    return cell_voltage_regs;
}
} // namespace io::adbms
