#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

using namespace std;

static array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> reg_group;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS>                 reg_group_success;
static const array<uint16_t, io::adbms::NUM_STAT_REG_GROUPS>              reg_groups{ {
    io::adbms::RDSTATA,
    io::adbms::RDSTATB,
    io::adbms::RDSTATC,
    io::adbms::RDSTATD,
    io::adbms::RDSTATE,
} };

namespace io::adbms
{

expected<void, ErrorCode> clearStatReg()
{
    RETURN_IF_ERR(sendCmd(CLRAUX));
    RETURN_IF_ERR(sendCmd(CLRFLAG));
    RETURN_IF_ERR(sendCmd(CLOVUV));
    return {};
}

void readStatusReg(
    array<StatusGroups, NUM_SEGMENTS>              &stat_regs,
    array<expected<void, ErrorCode>, NUM_SEGMENTS> &stat_regs_success)
{
    if (const auto ok = pollCellsAdcConversion(); !ok)
    {
        stat_regs_success.fill(ok);
        return;
    }

    if (const auto ok = pollTempAdcConversion(); !ok)
    {
        stat_regs_success.fill(ok);
        return;
    }
    

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        readRegGroup(reg_groups[group], reg_group, reg_group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!reg_group_success[seg])
            {
                stat_regs_success[seg] = reg_group_success[seg];
                continue;
            }

            switch (group)
            {
                case 0U:
                    memcpy(&stat_regs[seg].stat_a, reg_group[seg].data(), sizeof(STATA));
                    break;
                case 1U:
                    memcpy(&stat_regs[seg].stat_b, reg_group[seg].data(), sizeof(STATB));
                    break;
                case 2U:
                    memcpy(&stat_regs[seg].stat_c, reg_group[seg].data(), sizeof(STATC));
                    break;
                case 3U:
                    memcpy(&stat_regs[seg].stat_d, reg_group[seg].data(), sizeof(STATD));
                    break;
                case 4U:
                    memcpy(&stat_regs[seg].stat_e, reg_group[seg].data(), sizeof(STATE));
                    break;
                default:
                    stat_regs_success[seg] = unexpected(ErrorCode::ERROR_INDETERMINATE);
                    break;
            }
        }
    }
}

} // namespace io::adbms
