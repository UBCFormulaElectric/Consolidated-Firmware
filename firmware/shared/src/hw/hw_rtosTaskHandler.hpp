#pragma once
#include <cstdint.h>
#include "ccmsis_os2.h"

template <size_t StackWords> class StaticTask
{
  public:
    StaticTask(const char *name, osPriority_t priority, osThreadFunc_t func, void *arg = nullptr) : fn_(fn), arg_(arg)
    {
        // goot cpp practice is
        attr_.name       = name;
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_;
        attr_.stack_size = sizeof(stack_);
        attr_.priority   = priority;
    }

    // this is before the kernel start but after the initializer
    osThreadId_t start()
    {
        id_ = osThreadNew(func_, arg_, &attr_);
        return id_;
    }

    ofThreadId_t id() const { return id_; }

  private:
    alignas(8) uint32_t stack_[StackWords];
    osStaticThreadDef_t cb_{};
    osThreadAttr_t      attr_{};
    osThreadId_t        id_{};
    osThreadFunc_t      fn_{};
    void               *arg_{};
}