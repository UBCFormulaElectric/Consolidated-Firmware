#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "io_adbms.hpp"
#include <array>
#include <bitset>

namespace
{
std::array<std::bitset<4>, MAX_NUM_SEGMENTS> segment_health{};
io::semaphore                                health_mutex{ true };
} // namespace

namespace app::segments::health
{

void reset(size_t seg, Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    segment_health[seg].reset(static_cast<size_t>(bit));
}

void resetAll(Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.reset(static_cast<size_t>(bit));
}

void set(size_t seg, Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    segment_health[seg].set(static_cast<size_t>(bit));
}

void setAll(Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.set(static_cast<size_t>(bit));
}

bool isOk(size_t seg)
{
    const io::unique_semaphore lock{ health_mutex };
    return segment_health[seg].all();
}

} // namespace app::segments::health
