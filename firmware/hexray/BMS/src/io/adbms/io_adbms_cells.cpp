#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include <cstdint>

using namespace std;

static constexpr uint8_t MAX_NUM_ATTEMPTS = 10U;
static constexpr uint8_t CELLS_PER_GROUP  = 3U;

static const array<uint16_t, io::adbms::NUM_VOLT_REG_GROUPS> reg_groups{ {
    io::adbms::RDCVA,
    io::adbms::RDCVB,
    io::adbms::RDCVC,
    io::adbms::RDCVD,
    io::adbms::RDCVE,
} };

static const array<uint16_t, io::adbms::NUM_VOLT_REG_GROUPS> filtered_reg_groups{ {
    io::adbms::RDFCA,
    io::adbms::RDFCB,
    io::adbms::RDFCC,
    io::adbms::RDFCD,
    io::adbms::RDFCE,
} };



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

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        readRegGroup(reg_groups[group], shared_reg_group, shared_reg_group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!shared_reg_group_success[seg])
            {
                cell_voltage_regs[seg].fill(0U);
                comm_success[seg].fill(shared_reg_group_success[seg]);
                continue;
            }

            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                const size_t cell = group * CELLS_PER_GROUP + cell_in_group;
                if (cell < CELLS_PER_SEGMENT)
                {
                    const uint8_t  low     = shared_reg_group[seg][cell_in_group * 2U];
                    const uint8_t  high    = shared_reg_group[seg][cell_in_group * 2U + 1U];
                    const uint16_t voltage = static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8U);

                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        cell_voltage_regs[seg][cell] = 0U;
                        comm_success[seg][cell]      = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }

                    cell_voltage_regs[seg][cell] = voltage;
                    comm_success[seg][cell]      = {};
                }
            }
        }
    }
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

    for (size_t group = 0U; group < NUM_VOLT_REG_GROUPS; group++)
    {
        readRegGroup(filtered_reg_groups[group], shared_reg_group, shared_reg_group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!shared_reg_group_success[seg])
            {
                filtered_cell_voltage_regs[seg].fill(0U);
                comm_success[seg].fill(shared_reg_group_success[seg]);
                continue;
            }

            for (size_t cell_in_group = 0U; cell_in_group < CELLS_PER_GROUP; cell_in_group++)
            {
                const size_t cell = group * CELLS_PER_GROUP + cell_in_group;
                if (cell < CELLS_PER_SEGMENT)
                {
                    const uint8_t  low     = shared_reg_group[seg][cell_in_group * 2U];
                    const uint8_t  high    = shared_reg_group[seg][cell_in_group * 2U + 1U];
                    const uint16_t voltage = static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8U);

                    if (voltage == 0xFFFF || voltage == 0x8000)
                    {
                        filtered_cell_voltage_regs[seg][cell] = 0U;
                        comm_success[seg][cell]               = std::unexpected(ErrorCode::ERROR);
                        continue;
                    }

                    filtered_cell_voltage_regs[seg][cell] = voltage;
                    comm_success[seg][cell]               = {};
                }
            }
        }
    }
}

} // namespace io::adbms
