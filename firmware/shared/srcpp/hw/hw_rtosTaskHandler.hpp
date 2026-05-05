#pragma once
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include <string_view>

namespace hw::rtos
{
class StaticTask
{
  public:
    template <size_t SizeWords> struct StaticTaskStack
    {
        static_assert(SizeWords % 2 == 0, "Stack size must be a multiple of 8 bytes (2 words) for proper alignment.");
        alignas(8) uint32_t data[SizeWords] = {};
    };

  private:
    struct StaticTaskStackRef
    {
        uint32_t    *data;
        const size_t size;
        // ReSharper disable once CppNonExplicitConvertingConstructor
        template <size_t T> constexpr StaticTaskStackRef(StaticTaskStack<T> &s) : data(s.data), size(T) {}
        StaticTaskStackRef() = delete;
    };

  public:
    constexpr explicit StaticTask(
        const osPriority_t       priority,
        const std::string_view   name,
        const osThreadFunc_t     func,
        const StaticTaskStackRef stack)
      : prio_(priority), name_(name), fn_(func), stack_(stack)
    {
        // Runtime osThreadAttr_t assignments
        attr_.name       = name_.data();
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_.data;
        attr_.stack_size = stack.size * sizeof(uint32_t);
        attr_.priority   = prio_;
    }

    constexpr explicit StaticTask(
        const osPriority_t       priority,
        void                    *argument,
        const std::string_view   name,
        const osThreadFunc_t     func,
        const StaticTaskStackRef stack)
      : prio_(priority), name_(name), fn_(func), argument_(argument), stack_(stack)
    {
        // Runtime osThreadAttr_t assignments
        attr_.name       = name_.data();
        attr_.cb_mem     = &cb_;
        attr_.cb_size    = sizeof(cb_);
        attr_.stack_mem  = stack_.data;
        attr_.stack_size = stack.size * sizeof(uint32_t);
        attr_.priority   = prio_;
    }

    osThreadId_t start() const
    {
        id_ = osThreadNew(fn_, argument_, &attr_);
        return id_;
    }
    osThreadId_t id() const { return id_; }
    size_t       stackSize() const { return stack_.size; }

  private:
    const osPriority_t       prio_;
    const std::string_view   name_;
    osThreadFunc_t           fn_;
    mutable osThreadId_t     id_{};
    StaticTask_t             cb_{};
    void                    *argument_{};
    osThreadAttr_t           attr_{};
    const StaticTaskStackRef stack_;
};
} // namespace  hw::rtos