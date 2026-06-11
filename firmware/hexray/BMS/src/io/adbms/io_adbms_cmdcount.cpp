#include "io_adbms_cmdcount.hpp"
#include "io_adbms_spilink.hpp"   // addressCounts
#include "io_adbms_transport.hpp" // sendCmd (RSTCC recovery)

#include <algorithm>

using namespace std;

namespace io::adbms
{
namespace
{
    Segments<uint8_t> expected_cmd_count{};
    Segments<uint8_t> last_cmd_count_mismatches{};

    bool commandIncrements(const uint16_t cmd)
    {
        // ADCV_BASE = 0x0260, bits 9+6+5 constant; bits 8(RD), 7(CONT), 4(DCP), 2(RSTF), 1-0(OW) vary.
        if ((cmd & 0x0668U) == (ADCV_BASE & 0x0668U))
            return true;
        // ADSV_BASE = 0x0168, bits 8+6+5+3 constant; bits 7(CONT), 4(DCP), 1-0(OW) vary.
        if ((cmd & 0x076CU) == (ADSV_BASE & 0x076CU))
            return true;
        // ADAX_BASE = 0x0410, bits 10+4 constant; bits 8(OW), 7(PUP), 6(CH4), 3-0(CH3..CH0) vary.
        if ((cmd & 0x0630U) == (ADAX_BASE & 0x0630U))
            return true;
        // ADAX2_BASE = 0x0400, bits 10+9..4 all constant; only CH[3:0] (bits 3-0) vary.
        if ((cmd & 0x07F0U) == ADAX2_BASE)
            return true;

        switch (cmd)
        {
            // Writes
            case WRCFGA:
            case WRCFGB:
            case WRPWMA:
            case WRPWMB:
            // Clears
            case CLRCELL:
            case CLRFC:
            case CLRAUX:
            case CLRFLAG:
            case CLOVUV:
            // Polls
            case PLADC:
            case PLCADC:
            case PLSADC:
            case PLAUX:
            case PLAUX2:
            // Discharge / snapshot
            case MUTE:
            case UNMUTE:
            case SNAP:
            case UNSNAP:
                return true;
            default:
                return false;
        }
    }
} // namespace

Segments<uint8_t> getExpectedCmdCount()
{
    return expected_cmd_count;
}

void postTxUpdateCmdCount(const uint16_t cmd)
{
    if (cmd == RSTCC)
    {
        expected_cmd_count.fill(0);
        return;
    }
    if (!commandIncrements(cmd))
        return;
    // Advance each physical segment by the number of ends that addressed it this tx (0, 1, or 2).
    const auto counts = addressCounts();
    for (size_t i = 0; i < NUM_SEGMENTS; ++i)
    {
        for (uint8_t k = 0; k < counts[i]; ++k)
        {
            expected_cmd_count[i] =
                (expected_cmd_count[i] == 63U) ? 1U : static_cast<uint8_t>(expected_cmd_count[i] + 1U);
        }
    }
}

Segments<bool> detectCmdCountMismatches(const Segments<RegGroupPayload> &rxPhysical)
{
    Segments<bool> mismatches{};
    for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        mismatches[seg] = rxPhysical[seg].cmd_count() != expected_cmd_count[seg];
    }
    return mismatches;
}

void handleCmdCountMismatches(const Segments<bool> &mismatches)
{
    for (size_t i = 0; i < NUM_SEGMENTS; ++i)
    {
        last_cmd_count_mismatches[i] += mismatches[i];
    }
    if (!ranges::any_of(mismatches, [](const bool m) { return m; }))
        return;
    (void)sendCmd(RSTCC);
}

namespace misc
{
Segments<uint8_t> getCmdCountMismatches()
{
    return last_cmd_count_mismatches;
}
} // namespace misc

} // namespace io::adbms
