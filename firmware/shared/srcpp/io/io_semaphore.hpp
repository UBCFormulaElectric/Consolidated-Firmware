#pragma once

#include <stdbool.h>
#include <stdint.h>

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

    void take(uint32_t timeout) const;
    void give() const;

  private:
#ifdef TARGET_EMBEDDED
    StaticSemaphore_t freertos_semaphore_buf_;
    SemaphoreHandle_t freertos_semaphore_;
#else
    bool created_;
#endif
};

#ifdef TARGET_EMBEDDED
constexpr uint32_t MAX_TIMEOUT = portMAX_DELAY;
#else
constexpr uint32_t MAX_TIMEOUT = UINT32_MAX;
#endif
} // namespace io
