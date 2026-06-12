#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "io_semaphore.hpp"
#include <array>
#include <bitset>
#include <cassert>

namespace
{
std::array<std::bitset<app::segments::health::NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS> segment_health{};
} // namespace

io::semaphore health_lock{ true };

namespace app::segments::health
{

void setOrReset(const size_t seg, ErrorBit bit, bool has_error)
{
    assert(health_lock.is_held());
    segment_health[seg].set(static_cast<size_t>(bit), has_error);
}

void setOrResetAll(ErrorBit bit, bool has_error)
{
    assert(health_lock.is_held());
    for (auto &bs : segment_health)
        bs.set(static_cast<size_t>(bit), has_error);
}

bool getAnyError(const size_t seg)
{
    assert(health_lock.is_held());
    return segment_health[seg].any();
}

std::array<std::bitset<NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS> getAll()
{
    assert(health_lock.is_held());
    return segment_health;
}

} // namespace app::segments::health
