#pragma once

namespace can_handlers
{
[[noreturn]] void CanRXTask();
[[noreturn]] void CanPeriodicTXTask();

#ifdef USING_dimos_dev
constexpr uint16_t TASK_INTERVAL_100HZ = 1000;
constexpr uint16_t TASK_INTERVAL_1HZ = 1000;
#elif USING_dimos
constexpr uint8_t TASK_INTERVAL_100HZ = 10;
constexpr uint8_t TASK_INTERVAL_1HZ = 1000;
#endif
} // namespace can_handlers

