#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"

namespace app::segments::reach
{
using io::adbms::Segments;

Segments<result<bool>> check()
{
    Segments<result<bool>> out;
    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        out[seg] = true;
    }
    return out;
}

} // namespace app::segments::reach
