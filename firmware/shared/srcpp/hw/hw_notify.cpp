#include "hw_notify.hpp"

namespace hw::notify
{

void Notifier::wait()
{
    xTaskNotifyStateClear(nullptr);
    waitingTask_ = xTaskGetCurrentTaskHandle();
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    waitingTask_ = nullptr;
}

void Notifier::notifyIfWaiting()
{
    if (const TaskHandle_t task = waitingTask_; task != nullptr)
    {
        waitingTask_ = nullptr;
        xTaskNotifyGive(task);
    }
}

void Notifier::notifyIfWaitingFromISR()
{
    if (const TaskHandle_t task = waitingTask_; task != nullptr)
    {
        waitingTask_                        = nullptr;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(task, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

result<void> Notifier::waitFor(const uint32_t timeout_ms)
{
    xTaskNotifyStateClear(nullptr);
    waitingTask_ = xTaskGetCurrentTaskHandle();
    const uint32_t notified = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(timeout_ms));
    waitingTask_            = nullptr;
    if (notified == 0)
        return std::unexpected(ErrorCode::TIMEOUT);
    return {};
}

} // namespace hw::notify
