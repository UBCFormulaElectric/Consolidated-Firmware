#include "hw_notify.hpp"

namespace hw::notify
{

static constexpr EventBits_t NOTIFY_BIT = 1u << 0;

Notifier::Notifier() : handle_(xEventGroupCreateStatic(&storage_)) {}

void Notifier::wait()
{
    xEventGroupWaitBits(handle_, NOTIFY_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
}

result<void> Notifier::waitFor(const uint32_t timeout_ms)
{
    const EventBits_t bits =
        xEventGroupWaitBits(handle_, NOTIFY_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(timeout_ms));
    if ((bits & NOTIFY_BIT) == 0) return std::unexpected(ErrorCode::TIMEOUT);
    return {};
}

void Notifier::notify()
{
    xEventGroupSetBits(handle_, NOTIFY_BIT);
}

void Notifier::notifyFromISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(handle_, NOTIFY_BIT, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

} // namespace hw::notify
