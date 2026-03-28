#pragma once
#include <cstdint>
#include <cstddef>
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include <string>
#include "timers.h"
namespace hw::rtos
{
template <size_t StackWords> class StaticTask
{
  public:
    constexpr explicit StaticTask(osPriority_t priority, const char *name, osThreadFunc_t func)
      : prio_(priority), argument_(nullptr), name_(name), fn_(func)

    {
        // Runtime osThreadAttr_t assignments
        attr_.name       = name_;
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_;
        attr_.stack_size = StackWords * sizeof(uint32_t);
        attr_.priority   = prio_;
    }

    constexpr explicit StaticTask(osPriority_t priority, void *argument, const char *name, osThreadFunc_t func)
      : prio_(priority), argument_(argument), name_(name), fn_(func)

    {
        // Runtime osThreadAttr_t assignments
        attr_.name       = name_;
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_;
        attr_.stack_size = StackWords * sizeof(uint32_t);
        attr_.priority   = prio_;
    }

    osThreadId_t start()
    {
        id_ = osThreadNew(fn_, argument_, &attr_);
        return id_;
    }
    osThreadId_t id() const { return id_; }

  private:
    const osPriority_t prio_;
    const char        *name_;
    osThreadFunc_t     fn_;
    osThreadId_t       id_{};
    StaticTask_t       cb_{};
    void              *argument_{};
    osThreadAttr_t     attr_{};
    alignas(8) uint32_t stack_[StackWords];
};
} // namespace  hw::rtos