#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include <cstdint>

using namespace std;

static constexpr uint8_t MAX_NUM_ATTEMPTS = 10U;
static constexpr uint8_t CELLS_PER_GROUP  = 3U;

static array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> reg_group;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS>                 reg_group_success;

static array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> filtered_reg_group;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS>                 filtered_reg_group_success;

static const array<uint16_t, io::adbms::NUM_VOLT_REG_GROUPS> reg_groups{ {
    io::adbms::RDCVA,
    io::adbms::RDCVB,
    io::adbms::RDCVC,
    io::adbms::RDCVD,
    io::adbms::RDCVE,
} };

[[maybe_unused]] static const array<uint16_t, io::adbms::NUM_VOLT_REG_GROUPS> filtered_reg_groups{ {
    io::adbms::RDFCA,
    io::adbms::RDFCB,
    io::adbms::RDFCC,
    io::adbms::RDFCD,
    io::adbms::RDFCE,
} };

static void readCellVoltageRegsImpl(
    const array<uint16_t, io::adbms::NUM_VOLT_REG_GROUPS>                   &groups,
    array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS>      &buf,
    array<expected<void, ErrorCode>, io::NUM_SEGMENTS>                      &buf_success,
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>         &cell_voltage_regs,
    array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &comm_success)
{
    for (size_t group = 0U; group < io::adbms::NUM_VOLT_REG_GROUPS; group++)
    {
        io::adbms::readRegGroup(groups[group], buf, buf_success);

        const size_t cell_start    = group * CELLS_PER_GROUP;
        const size_t cells_in_group = (cell_start < io::CELLS_PER_SEGMENT)
            ? min<size_t>(CELLS_PER_GROUP, io::CELLS_PER_SEGMENT - cell_start)
            : 0U;

        for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
        {
            if (!buf_success[seg])
            {
                cell_voltage_regs[seg].fill(0U);
                comm_success[seg].fill(buf_success[seg]);
                continue;
            }

            for (size_t i = 0U; i < cells_in_group; i++)
            {
                const size_t   cell    = cell_start + i;
                const uint8_t  low     = buf[seg][i * 2U];
                const uint8_t  high    = buf[seg][i * 2U + 1U];
                const uint16_t voltage = static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8U);

                if (voltage == 0xFFFF || voltage == 0x8000)
                {
                    cell_voltage_regs[seg][cell] = 0U;
                    comm_success[seg][cell]      = std::unexpected(ErrorCode::ERROR);
                }
                else
                {
                    cell_voltage_regs[seg][cell] = voltage;
                    comm_success[seg][cell]      = {};
                }
            }
        }
    }
}

namespace io::adbms
{

expected<void, ErrorCode> clearCellVoltageReg()
{
    return sendCmd(CLRCELL);
}

expected<void, ErrorCode> clearFilteredCellVoltageReg()
{
    return sendCmd(CLRFC);
}

expected<void, ErrorCode> startCellsAdcConversion()
{
    return sendCmd(ADCV_BASE | RD | CONT);
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

void readCellVoltageReg(
    array<array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS>                  &cell_voltage_regs,
    array<array<expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success)
{
    const auto poll_ok = pollCellsAdcConversion();

    if (!poll_ok)
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            comm_success[seg].fill(poll_ok);
        }
        return;
    }

    readCellVoltageRegsImpl(reg_groups, reg_group, reg_group_success, cell_voltage_regs, comm_success);
}

void readFilteredCellVoltageReg(
    array<array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS>                  &filtered_cell_voltage_regs,
    array<array<expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success)
{
    const auto poll_ok = pollCellsAdcConversion();

    if (!poll_ok)
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            comm_success[seg].fill(poll_ok);
        }
        return;
    }

    readCellVoltageRegsImpl(
        filtered_reg_groups, filtered_reg_group, filtered_reg_group_success,
        filtered_cell_voltage_regs, comm_success);
}



} // namespace io::adbms
