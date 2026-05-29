#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include <array>
#include <bitset>

namespace
{
inline constexpr size_t NUM_HEALTH_BITS = static_cast<size_t>(app::segments::health::ErrorBit::NUM_ERROR_BITS);

std::array<std::bitset<NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS> segment_health{};
// health mutex protects the segment_health array
io::semaphore health_mutex{ true };
} // namespace

namespace app::segments::health
{

void resetAll(ErrorBit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.reset(static_cast<size_t>(bit));
}

void setAll(ErrorBit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.set(static_cast<size_t>(bit));
}

void setOrReset(const size_t seg, ErrorBit bit, bool has_error)
{
    const io::unique_semaphore lock{ health_mutex };
    segment_health[seg].set(static_cast<size_t>(bit), has_error);
}

bool getError(const size_t seg, ErrorBit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    return segment_health[seg].test(static_cast<size_t>(bit));
}

bool getAnyError(const size_t seg) {
    const io::unique_semaphore lock{ health_mutex };
    return segment_health[seg].any();
}

} // namespace app::segments::health
