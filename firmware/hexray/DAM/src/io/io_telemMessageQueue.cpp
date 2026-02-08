#include "io_telemMessageQueue.hpp"
#include "io_queue.hpp"

namespace
{
constexpr size_t QUEUE_SIZE = 52;

void overflowCallback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("Telem Message TX Queue overflow");
}

void overflowClearCallback()
{
    LOG_INFO("Telem Message TX Queue overflow cleared");
}

io::queue<io::CanMsg, QUEUE_SIZE> queue{ "Telem Message TX Queue", overflowCallback, overflowClearCallback };
} // namespace

namespace io::telemMessageQueue
{

void init()
{
    queue.init();
}

std::expected<void, ErrorCode> push(const io::CanMsg &msg)
{
    return queue.push(msg);
}

std::expected<io::CanMsg, ErrorCode> pop()
{
    return queue.pop();
}

} // namespace io::telemMessageQueue
