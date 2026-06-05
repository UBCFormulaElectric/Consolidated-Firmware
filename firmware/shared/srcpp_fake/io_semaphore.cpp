#include "io_semaphore.hpp"

io::semaphore::semaphore(bool priority_inheritance_protocol_on, const uint32_t sysview_id){};
void io::semaphore::take(uint32_t timeout) const {};
void io::semaphore::give() const {};
bool io::semaphore::is_held() const
{
    return false;
}