#pragma once

#include <limits>
#include <cstdint>

#ifdef TARGET_EMBEDDED
#include "cmsis_os2.h"
#include <FreeRTOS.h>
#endif

namespace io::semaphore
{
class Semaphore
{
    uint32_t _sysview_id;

  public:
    explicit Semaphore(bool priority_inheritance_protocol_on, const uint32_t sysview_id = 0);

    void take(uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;
    void give() const;

    /**
     * @return whether the lock is being held by the current task
     */
    bool is_held() const;

  private:
#ifdef TARGET_EMBEDDED
    StaticSemaphore_t freertos_semaphore_buf_{};
    osMutexId_t       mutex_id_{};
    osSemaphoreId_t   semaphore_id_{};
#else
    bool created_;
#endif
};

class unique_semaphore
{
    const Semaphore &_sem;

  public:
    explicit unique_semaphore(const Semaphore &sem, const uint32_t timeout = std::numeric_limits<uint32_t>::max())
      : _sem(sem)
    {
        _sem.take(timeout);
    }
    ~unique_semaphore() { _sem.give(); }
};
} // namespace io::semaphore
