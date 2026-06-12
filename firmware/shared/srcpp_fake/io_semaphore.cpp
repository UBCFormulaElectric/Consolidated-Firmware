#include "io_semaphore.hpp"

#include <unordered_set>

namespace
{
// Tracks which semaphore instances are currently held. The test build is
// single-threaded, so a simple set keyed by instance is enough to make the
// is_held() assertions in application code meaningful.
std::unordered_set<const io::semaphore *> &held_semaphores()
{
    static std::unordered_set<const io::semaphore *> held;
    return held;
}
} // namespace

io::semaphore::semaphore(bool priority_inheritance_protocol_on, const uint32_t sysview_id){};
void io::semaphore::take(uint32_t timeout) const
{
    held_semaphores().insert(this);
};
void io::semaphore::give() const
{
    held_semaphores().erase(this);
};
bool io::semaphore::is_held() const
{
    return held_semaphores().contains(this);
}
