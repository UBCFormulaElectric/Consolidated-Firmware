#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

namespace hw
{
class MutexGuard
{
    SemaphoreHandle_t m;

  public:
    explicit MutexGuard(SemaphoreHandle_t m_) : m(m_) { xSemaphoreTake(m, portMAX_DELAY); }
    ~MutexGuard() { xSemaphoreGive(m); }

    MutexGuard(const MutexGuard &)            = delete;
    MutexGuard &operator=(const MutexGuard &) = delete;
    MutexGuard(MutexGuard &&)                 = delete;
    MutexGuard &operator=(MutexGuard &&)      = delete;
};
} // namespace hw
