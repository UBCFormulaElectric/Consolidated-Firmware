#include "io_telemMessageQueue.hpp"
#include "io_queue.hpp"

namespace io::telemMessageQueue
{

// TODO: Instantiate io::queue<io::CanMsg, QUEUE_SIZE> here

void init()
{
    // TODO: Initialize the queue
}

std::expected<void, ErrorCode> push(const io::CanMsg &msg)
{
    // TODO: Push message to queue
    return {};
}

std::expected<io::CanMsg, ErrorCode> pop()
{
    // TODO: Pop message from queue (blocking)
    return std::unexpected(ErrorCode::ERROR);
}

} // namespace io::telemMessageQueue
