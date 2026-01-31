#pragma once

#include "cmsis_os.h"
#include <cassert>
#include <array>

#include "io_log.hpp"
#include "util_errorCodes.hpp"

#include <expected>

namespace io
{
template <typename T, size_t QUEUE_SIZE> class queue
{
    constexpr static size_t               QUEUE_SIZE_BYTES = sizeof(T) * QUEUE_SIZE;
    osMessageQueueId_t                    queue_id         = nullptr;
    StaticQueue_t                         queue_control_block{};
    std::array<uint8_t, QUEUE_SIZE_BYTES> queue_buf{};

    // inputs
    const osMessageQueueAttr_t queue_attr;
    void (*const overflow_callback)(uint32_t){};
    void (*const overflow_clear_callback)(){};

    // private vars
    uint32_t overflow_count = 0;
    bool     overflow_flag  = false;

  public:
    consteval explicit queue(
        const char *name,
        void (*const in_overflow_callback)(uint32_t),
        void (*const in_overflow_clear_callback)())
      : queue_attr({
            .name      = name,
            .attr_bits = 0,
            .cb_mem    = &this->queue_control_block,
            .cb_size   = sizeof(StaticQueue_t),
            .mq_mem    = this->queue_buf.data(),
            .mq_size   = QUEUE_SIZE_BYTES,
        }),
        overflow_callback(in_overflow_callback),
        overflow_clear_callback(in_overflow_clear_callback)
    {
    }

    /**
     * Initialize and start the CAN peripheral.
     * Note: this breaks RAII but embedded moment
     */
    void init() { this->queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(T), &this->queue_attr); }

    /**
     * Enqueue a CAN msg to be transmitted on the bus.
     * Does not block, calls `overflow_callback` if queue is full.
     * @param msg CAN msg to be TXed.
     */
    ExitCode push(const T &msg)
    {
        assert(queue_id != nullptr);
        if (const osStatus_t s = osMessageQueuePut(this->queue_id, &msg, 0, 0); s != osOK)
        {
            if (!this->overflow_flag)
            {
                LOG_WARN("%s overflow: %d", this->queue_attr.name, s);
                this->overflow_flag = true;
            }
            this->overflow_callback(++this->overflow_count);
            return ExitCode::EXIT_CODE_ERROR;
        }
        this->overflow_clear_callback();
        this->overflow_flag = false;
        return ExitCode::EXIT_CODE_OK;
    }

    /**
     * Pops a CAN msg from the queue. Blocks until a msg exists in the queue.
     */
    [[nodiscard]] std::expected<T, ExitCode> pop()
    {
        assert(queue_id != nullptr);
        T msg{};
        if (const osStatus_t s = osMessageQueueGet(this->queue_id, &msg, nullptr, osWaitForever); s != osOK)
        {
            return std::unexpected(ExitCode::EXIT_CODE_ERROR);
        }
        return msg;
    }
};
} // namespace io
