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

} // namespace hw::notify
