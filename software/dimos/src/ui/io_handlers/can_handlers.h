#pragma once

namespace can_handlers
{
void CanRXTask();
void CanPeriodicTXTask();
void CanTx100Hz();
void CanTx1Hz();

#ifdef USING_dimos_dev
constexpr uint16_t TASK_INTERVAL_100HZ = 1000;
constexpr uint16_t TASK_INTERVAL_1HZ   = 1000;
#elif USING_dimos
constexpr uint16_t TASK_INTERVAL_100HZ = 10;
constexpr uint16_t TASK_INTERVAL_1HZ   = 1000;
#endif
} // namespace can_handlers
