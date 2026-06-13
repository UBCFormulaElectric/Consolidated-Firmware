#include "io_semaphore.hpp"

#include <unordered_set>

namespace
{

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
