#pragma once
#include <cstdint>
#include <cstddef>
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include <string>
#include "timers.h"
namespace hw::rtos
{
class StaticTask
{
  public:
    constexpr explicit StaticTask(
        uint32_t      *stack_mem,
        size_t         stack_words,
        osPriority_t   priority,
        const char    *name,
        osThreadFunc_t func)
      : size_(stack_mem), stack_(stack_words), prio_(priority), name_(name), fn_(func)
    {
        // Runtime osThreadAttr_t assignments
        attr_.name       = name_;
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = size_;
        attr_.stack_size = stack_ * sizeof(uint32_t);
        attr_.priority   = prio_;
    }

    osThreadId_t start()
    {
        id_ = osThreadNew(fn_, nullptr, &attr_);
        return id_;
    }
    osThreadId_t id() const { return id_; }

  private:
    uint32_t *const    size_;
    const size_t       stack_;
    const osPriority_t prio_;
    const char        *name_;
    osThreadFunc_t     fn_;
    osThreadId_t       id_{};
    StaticTask_t       cb_{};
    osThreadAttr_t     attr_{};
};
} // namespace  hw::rtos
