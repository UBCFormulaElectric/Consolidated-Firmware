#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"

namespace app::segments::reach
{
using io::adbms::Segments;

Segments<result<bool>> check()
{
    const io::adbms::SpiBusReach bus = io::adbms::misc::getSpiBusReach();

    // A segment is reachable if it falls within reach of either end of the
    // daisy chain: from the low side (segments [0, ls_reach)) or from the high
    // side (segments [NUM_SEGMENTS - hs_reach, NUM_SEGMENTS)).
    Segments<result<bool>> per_seg;
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        const bool reachable_from_ls = seg < bus.ls_reach;
        const bool reachable_from_hs = seg >= static_cast<size_t>(NUM_SEGMENTS - bus.hs_reach);
        per_seg[seg]                 = reachable_from_ls || reachable_from_hs;
    }
    return per_seg;
}

} // namespace app::segments::reach
