#pragma once

#include <limits>
#include <cstdint>

#ifdef TARGET_EMBEDDED
#include <FreeRTOS.h>
#include "semphr.h"
#include <portmacro.h>
#endif

namespace io
{
class semaphore
{
  public:
    explicit semaphore(bool priority_inheritance_protocol_on);

    void take(uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;
    void give() const;

    /**
     * @return whether the lock is being held by the current task
     */
    bool is_held() const;

  private:
#ifdef TARGET_EMBEDDED
    StaticSemaphore_t freertos_semaphore_buf_{};
    SemaphoreHandle_t freertos_semaphore_;
#else
    bool created_;
#endif
};

class unique_semaphore
{
    const semaphore &_sem;

  public:
    explicit unique_semaphore(const semaphore &sem, const uint32_t timeout = std::numeric_limits<uint32_t>::max())
      : _sem(sem)
    {
        _sem.take(timeout);
    }
    ~unique_semaphore() { _sem.give(); }
};
} // namespace io
