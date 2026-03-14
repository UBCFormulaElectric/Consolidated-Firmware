#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

using namespace std;

static array<array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> reg_group;
static array<expected<void, ErrorCode>, io::NUM_SEGMENTS> reg_group_success;
static const array<uint16_t, io::adbms::NUM_STAT_REG_GROUPS> reg_groups{ {
    io::adbms::RDSTATA,
    io::adbms::RDSTATB,
    io::adbms::RDSTATC,
    io::adbms::RDSTATD,
    io::adbms::RDSTATE,
} };

static_assert(sizeof(io::adbms::STATA) == io::adbms::REG_GROUP_SIZE);
static_assert(sizeof(io::adbms::STATB) == io::adbms::REG_GROUP_SIZE);
static_assert(sizeof(io::adbms::STATC) == io::adbms::REG_GROUP_SIZE);
static_assert(sizeof(io::adbms::STATD) == io::adbms::REG_GROUP_SIZE);
static_assert(sizeof(io::adbms::STATE) == io::adbms::REG_GROUP_SIZE);
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
    array<StatusGroups, NUM_SEGMENTS> &stat_regs,
    array<expected<void, ErrorCode>, NUM_SEGMENTS> &stat_regs_success)
{
    const auto poll_ok = pollTempAdcConversion();

    if (!poll_ok)
    {
        stat_regs_success.fill(poll_ok);
        return;
    }

    stat_regs_success.fill({});

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        readRegGroup(reg_groups[group], reg_group, reg_group_success);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!stat_regs_success[seg])
            {
                continue;
            }

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
