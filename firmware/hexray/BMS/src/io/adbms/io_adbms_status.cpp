#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

using namespace std;

namespace io::adbms
{

expected<void, ErrorCode> clearStatReg()
{
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> clr_regs{};
    
    RETURN_IF_ERR(sendCmd(CLRAUX));


    RETURN_IF_ERR(sendCmd(CLRFLAG));
    RETURN_IF_ERR(sendCmd(CLOVUV));
    return {};
}

Segments<std::expected<StatusGroups, ErrorCode>> readStatusReg()
{
    static constexpr array<uint16_t, NUM_STAT_REG_GROUPS> reg_groups{ { RDSTATA, RDSTATB, RDSTATC, RDSTATD, RDSTATE } };
    Segments<std::expected<StatusGroups, ErrorCode>>      stat_regs;
    // TODO: TEST (Unsure if this poll is needed)
    if (const auto ok = pollCellsAdcConversion(); !ok)
    {
        stat_regs.fill(unexpected(ok.error()));
        return stat_regs;
    }
    if (const auto ok = pollAuxAdcConversion(); !ok)
    {
        stat_regs.fill(unexpected(ok.error()));
        return stat_regs;
    }

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            if (!out[seg])
            {
                stat_regs[seg] = unexpected(out[seg].error());
                continue;
            }

            switch (group)
            {
                case 0U:
                    stat_regs[seg]->stat_a = *reinterpret_cast<const STATA *>(out[seg]->data());
                    break;
                case 1U:
                    stat_regs[seg]->stat_b = *reinterpret_cast<const STATB *>(out[seg]->data());
                    break;
                case 2U:
                    stat_regs[seg]->stat_c = *reinterpret_cast<const STATC *>(out[seg]->data());
                    break;
                case 3U:
                    stat_regs[seg]->stat_d = *reinterpret_cast<const STATD *>(out[seg]->data());
                    break;
                case 4U:
                    stat_regs[seg]->stat_e = *reinterpret_cast<const STATE *>(out[seg]->data());
                    break;
                default:
                    stat_regs[seg] = unexpected(ErrorCode::ERROR_INDETERMINATE);
                    break;
            }
        }
    }
    return stat_regs;
}

} // namespace io::adbms
