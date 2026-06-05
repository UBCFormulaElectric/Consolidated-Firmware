#include "io_notify.hpp"

namespace io::notify
{

static constexpr EventBits_t NOTIFY_BIT = 1u << 0;

Notifier::Notifier() : handle_(xEventGroupCreateStatic(&storage_)) {}

void Notifier::wait() const
{
    xEventGroupWaitBits(handle_, NOTIFY_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
}

void Notifier::notify() const
{
    xEventGroupSetBits(handle_, NOTIFY_BIT);
}

void Notifier::notifyFromISR() const
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(handle_, NOTIFY_BIT, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Notifier::clear() const
{
    xEventGroupClearBits(handle_, NOTIFY_BIT);
}

} // namespace io::notify
