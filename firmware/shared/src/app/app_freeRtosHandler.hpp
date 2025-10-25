// osThreadId_t         Task1kHzHandle;
// uint32_t             Task1kHzBuffer[512];
// osStaticThreadDef_t  Task1kHzControlBlock;
// const osThreadAttr_t Task1kHz_attributes = {
//     .name       = "Task1kHz",
//     .cb_mem     = &Task1kHzControlBlock,
//     .cb_size    = sizeof(Task1kHzControlBlock),
//     .stack_mem  = &Task1kHzBuffer[0],
//     .stack_size = sizeof(Task1kHzBuffer),
//     .priority   = (osPriority_t)osPriorityRealtime,
// };

// Should be simplified to: osThread(Task1kHz, 512, osPriorityRealtime)


#pragma once
#include <cstdint>
#include "cmsis_os2.h"

extern "C" {
#include "cmsis_os.h"
}
#ifndef osStaticThreadDef_t
struct osStaticThreadDef_t { uint8_t _pad[128]; };
#endif

namespace rtosThread {

using TaskFn = osThreadFunc_t;  

// Simple intrusive registry
struct ITask {
  virtual ~ITask() = default;
  virtual osThreadId_t create() = 0;
  ITask* next = nullptr;
};

struct Registry {
  static Registry& instance() { static Registry R; return R; }
  void add(ITask* t) { t->next = head; head = t; }
  void create_all() { for (ITask* t = head; t; t = t->next) t->create(); }
private:
  ITask* head = nullptr;
};

// Static, pre-allocated task wrapper (stack size in WORDS, like CubeMX)
template <size_t StackWords>
struct StaticTask : public ITask {
  const char*   name;
  osPriority_t  priority;
  TaskFn        fn;
  void*         arg;

  alignas(8) uint32_t stack[StackWords];
  osStaticThreadDef_t cb;
  osThreadAttr_t      attr{};
  osThreadId_t        handle{};

  StaticTask(const char* n, osPriority_t p, TaskFn f, void* a=nullptr)
  : name(n), priority(p), fn(f), arg(a) {
    attr.name       = name;
    attr.cb_mem     = &cb;
    attr.cb_size    = sizeof(cb);
    attr.stack_mem  = stack;
    attr.stack_size = sizeof(stack);
    attr.priority   = priority;
    Registry::instance().add(this);
  }

  osThreadId_t create() override { handle = osThreadNew(fn, arg, &attr); return handle; }
};

// Create everything that was registered
inline void create_all_tasks() { Registry::instance().create_all(); }

} // namespace rtosThread

// One-line file-scope declaration/registration
#define RTOSTHREAD_JOIN2(a,b) a##b
#define RTOSTHREAD_JOIN(a,b)  RTOSTHREAD_JOIN2(a,b)
#define RTOSTHREAD_DEFINE_TASK(stack_words, name, priority, fn, arg) \
  static rtosThread::StaticTask<(stack_words)> \
  RTOSTHREAD_JOIN(_rtosThread_task_, __COUNTER__){ (name), (osPriority_t)(priority), (fn), (void*)(arg) }
