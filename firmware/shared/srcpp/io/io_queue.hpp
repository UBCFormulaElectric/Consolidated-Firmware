#pragma once

#ifdef TARGET_EMBEDDED
#include "cmsis_os.h"
#elif TARGET_TEST
#include <queue>
#endif
#include <cassert>
#include <limits>
#include <array>
#include <cstdint>

#include "io_log.hpp"
#include "util_errorCodes.hpp"

#include <expected>

namespace io
{
template <typename T, size_t QUEUE_SIZE> class queue
{
    constexpr static size_t QUEUE_SIZE_BYTES = sizeof(T) * QUEUE_SIZE;

#ifdef TARGET_EMBEDDED
    osMessageQueueId_t                    queue_id = nullptr;
    StaticQueue_t                         queue_control_block{};
    std::array<uint8_t, QUEUE_SIZE_BYTES> queue_buf{};
    const osMessageQueueAttr_t            queue_attr;
#elif TARGET_TEST
    std::queue<T> q{};
#endif

    void (*const overflow_callback)(uint32_t){};
    void (*const overflow_clear_callback)(){};

  public:
    explicit queue(
        [[maybe_unused]] const char *name,
        void (*const in_overflow_callback)(uint32_t),
        void (*const in_overflow_clear_callback)())
      :
#ifdef TARGET_EMBEDDED
        queue_attr({
            .name      = name,
            .attr_bits = 0,
            .cb_mem    = &this->queue_control_block,
            .cb_size   = sizeof(StaticQueue_t),
            .mq_mem    = this->queue_buf.data(),
            .mq_size   = QUEUE_SIZE_BYTES,
        }),
#endif
        overflow_callback(in_overflow_callback),
        overflow_clear_callback(in_overflow_clear_callback)
    {
    }

    // private vars
    uint32_t overflow_count = 0;
    bool     overflow_flag  = false;

    /**
     * Initialize and start the CAN peripheral.
     * Note: this breaks RAII but embedded moment
     */
    void init()
    {
#ifdef TARGET_EMBEDDED
        this->queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(T), &this->queue_attr);
#endif
    }

    /**
     * Enqueue a CAN msg to be transmitted on the bus.
     * Does not block, calls `overflow_callback` if queue is full.
     * @param msg CAN msg to be TXed.
     */
    [[nodiscard]] std::expected<void, ErrorCode> push(const T &msg)
    {
#ifdef TARGET_EMBEDDED
        assert(queue_id != nullptr);
        if (const osStatus_t s = osMessageQueuePut(this->queue_id, &msg, 0, 0); s != osOK)
        {
            if (!this->overflow_flag)
            {
                // LOG_WARN("%s overflow: %d", this->queue_attr.name, s);
                this->overflow_flag = true;
            }
            this->overflow_callback(++this->overflow_count);
            return std::unexpected(ErrorCode::ERROR);
        }
        this->overflow_clear_callback();
        this->overflow_flag = false;
#elif TARGET_TEST
        q.push(msg);
#endif
        return {};
    }

    /**
     * Pops a CAN msg from the queue. Blocks until a msg exists in the queue.
     */
    [[nodiscard]] std::expected<T, ErrorCode> pop(const uint32_t timeout = std::numeric_limits<uint32_t>::max())
    {
#ifdef TARGET_EMBEDDED
        assert(queue_id != nullptr);
        T msg;
        if (const osStatus_t s = osMessageQueueGet(this->queue_id, &msg, nullptr, timeout); s != osOK)
        {
            return std::unexpected(ErrorCode::ERROR);
        }
        return msg;
#elif TARGET_TEST
        UNUSED(timeout);
        const auto out = q.front();
        q.pop();
        return out;
#endif
    }
};
} // namespace io
