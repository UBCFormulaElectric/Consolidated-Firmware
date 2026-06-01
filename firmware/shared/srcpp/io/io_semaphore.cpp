#include "io_semaphore.hpp"
#include <cassert>

#ifdef TARGET_EMBEDDED

io::semaphore::semaphore(const bool priority_inheritance_protocol_on)
{
    if (priority_inheritance_protocol_on)
    {
        freertos_semaphore_ = xSemaphoreCreateMutexStatic(&freertos_semaphore_buf_);
    }
    else
    {
        freertos_semaphore_ = xSemaphoreCreateBinaryStatic(&freertos_semaphore_buf_);
    }

    assert(freertos_semaphore_ != nullptr);
    give();
}

void io::semaphore::give() const
{
    xSemaphoreGive(freertos_semaphore_);
}
void io::semaphore::take(const uint32_t timeout) const
{
    const auto take_success = xSemaphoreTake(freertos_semaphore_, timeout);
    assert(take_success == pdTRUE);
}

bool io::semaphore::is_held() const
{
    return xSemaphoreGetMutexHolder(freertos_semaphore_) == xTaskGetCurrentTaskHandle();
}

#else

io::semaphore::semaphore(bool) : created_(true) {}
void io::semaphore::give() const {}
void io::semaphore::take(uint32_t) const {}

#endif