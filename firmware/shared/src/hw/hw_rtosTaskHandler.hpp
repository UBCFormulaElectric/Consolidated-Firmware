#pragma once
#include <cstdint>
#include <cstddef>
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include <string> 
#include "timers.h"
// Template class wrapper for the RTOS declaration in main.c
template <size_t StackWords, osPriority_t Priority, const char* Name> class StaticTask
{
  public:
    StaticTask(osThreadFunc_t func, void *arg = nullptr) : fn_(func), arg_(arg)
    {
        // good cpp practice is
        attr_.name       = Name;
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_;
        attr_.stack_size = sizeof(stack_);
       
        attr_.priority   = Priority;
    }

    // Ran after the kernal is initialized where the board's task thread are generated
    osThreadId_t start()
    {
        id_ = osThreadNew(fn_, arg_, &attr_);
        return id_;
    }

    osThreadId_t id() const { return id_; }

  private:
    alignas(8) uint32_t stack_[StackWords];
    StaticTask_t        cb_{};
    osThreadAttr_t      attr_{};
    osThreadId_t        id_{};
    osThreadFunc_t      fn_{};
    void               *arg_{};
};