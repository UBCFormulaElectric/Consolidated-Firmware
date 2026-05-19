#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include <cstring>

using namespace std;

namespace io::adbms
{

result<void> clear::StatReg()
{
    constexpr Segments<RegBuffer> clr_regs{};
    RETURN_IF_ERR(sendCmd(CLRAUX));
    RETURN_IF_ERR(writeRegGroup(CLRFLAG, clr_regs));
    RETURN_IF_ERR(writeRegGroup(CLOVUV, clr_regs));
    return {};
}

result<Segments<StatusGroups>> readStatusRegs()
{
    static constexpr array<uint16_t, NUM_STAT_REG_GROUPS> reg_groups{ { RDSTATA, RDSTATB, RDSTATC, RDSTATD, RDSTATE } };
    Segments<StatusGroups>                                stat_regs;
    // TODO: TEST (Unsure if this poll is needed)
    if (const auto poll_ok = pollCellsAdcConversion(); !poll_ok)
        return unexpected(poll_ok.error());
    if (const auto poll_ok = pollAuxAdcConversion(); !poll_ok)
        return unexpected(poll_ok.error());

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            switch (group)
            {
                case 0U:
                    stat_regs[seg].stat_a = out[seg] ? *reinterpret_cast<const STATA *>(out[seg]->data())
                                                     : result<STATA>{ unexpected(out[seg].error()) };
                    break;
                case 1U:
                    stat_regs[seg].stat_b = out[seg] ? *reinterpret_cast<const STATB *>(out[seg]->data())
                                                     : result<STATB>{ unexpected(out[seg].error()) };
                    break;
                case 2U:
                    stat_regs[seg].stat_c = out[seg] ? *reinterpret_cast<const STATC *>(out[seg]->data())
                                                     : result<STATC>{ unexpected(out[seg].error()) };
                    break;
                case 3U:
                    stat_regs[seg].stat_d = out[seg] ? *reinterpret_cast<const STATD *>(out[seg]->data())
                                                     : result<STATD>{ unexpected(out[seg].error()) };
                    break;
                case 4U:
                    stat_regs[seg].stat_e = out[seg] ? *reinterpret_cast<const STATE *>(out[seg]->data())
                                                     : result<STATE>{ unexpected(out[seg].error()) };
                    break;
                default:
                    break;
            }
        }
    }
    return stat_regs;
}

} // namespace io::adbms
