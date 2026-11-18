#include "io_semaphore.hpp"

#include <unordered_set>

namespace
{

std::unordered_set<const io::semaphore::Semaphore *> &held_semaphores()
{
    static std::unordered_set<const io::semaphore::Semaphore *> held;
    return held;
}
} // namespace

io::semaphore::Semaphore::Semaphore(bool priority_inheritance_protocol_on, const uint32_t sysview_id){};
void io::semaphore::Semaphore::take(uint32_t timeout) const
{
    held_semaphores().insert(this);
};
void io::semaphore::Semaphore::give() const
{
    held_semaphores().erase(this);
};
bool io::semaphore::Semaphore::is_held() const
{
    return held_semaphores().contains(this);
}
