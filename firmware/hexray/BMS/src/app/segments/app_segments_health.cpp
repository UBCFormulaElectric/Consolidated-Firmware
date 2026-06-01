#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "io_semaphore.hpp"
#include <array>
#include <bitset>
#include <cassert>

namespace
{
inline constexpr size_t NUM_HEALTH_BITS = static_cast<size_t>(app::segments::health::ErrorBit::NUM_ERROR_BITS);

std::array<std::bitset<NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS> segment_health{};
} // namespace

io::semaphore health_lock{ true };

namespace app::segments::health
{

void resetAll(ErrorBit bit)
{
    assert(health_lock.is_held());
    for (auto &bs : segment_health)
        bs.reset(static_cast<size_t>(bit));
}

void setAll(ErrorBit bit)
{
    assert(health_lock.is_held());
    for (auto &bs : segment_health)
        bs.set(static_cast<size_t>(bit));
}

void setOrReset(const size_t seg, ErrorBit bit, bool has_error)
{
    assert(health_lock.is_held());
    segment_health[seg].set(static_cast<size_t>(bit), has_error);
}

bool getError(const size_t seg, ErrorBit bit)
{
    assert(health_lock.is_held());
    return segment_health[seg].test(static_cast<size_t>(bit));
}

bool getAnyError(const size_t seg)
{
    assert(health_lock.is_held());
    return segment_health[seg].any();
}

} // namespace app::segments::health
