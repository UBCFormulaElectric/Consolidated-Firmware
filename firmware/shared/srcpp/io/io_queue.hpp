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
namespace internal
{
#if defined(TARGET_EMBEDDED)
    template <typename T, size_t QUEUE_SIZE> class queueImpl
    {
        constexpr static size_t QUEUE_SIZE_BYTES = sizeof(T) * QUEUE_SIZE;

        osMessageQueueId_t                    queue_id = nullptr;
        StaticQueue_t                         queue_control_block{};
        std::array<uint8_t, QUEUE_SIZE_BYTES> queue_buf{};
        const osMessageQueueAttr_t            queue_attr;

      public:
        /**
         * @brief Set up the static queue attributes
         * @param name Queue name
         */
        explicit queueImpl([[maybe_unused]] const char *name)
          : queue_attr({
                .name      = name,
                .attr_bits = 0,
                .cb_mem    = &this->queue_control_block,
                .cb_size   = sizeof(StaticQueue_t),
                .mq_mem    = this->queue_buf.data(),
                .mq_size   = QUEUE_SIZE_BYTES,
            })
        {
        }

        /**
         * @brief Create the RTOS queue on the static buffers
         */
        void init() { 
            this->queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(T), &this->queue_attr); 
        }

        /**
         * @brief Enqueue without blocking
         * @return false if the queue is full
         */
        bool put(const T &msg) {
            assert(queue_id != nullptr);
            return osMessageQueuePut(this->queue_id, &msg, 0, 0) == osOK;
        }

        /**
         * @brief Dequeue into `out`, blocking for up to `timeout` ticks
         * @return false on timeout
         */
        bool get(T &out, const uint32_t timeout) {
            assert(queue_id != nullptr);
            return osMessageQueueGet(this->queue_id, &out, nullptr, timeout) == osOK;
        }

        /**
         * @return number of elements currently in the queue
         */
        size_t count() const { 
            assert(queue_id != nullptr);
            return osMessageQueueGetCount(this->queue_id);
        }
    };
#elif defined(TARGET_TEST)
    template <typename T, size_t QUEUE_SIZE> class queueImpl
    {
        std::queue<T> q{};

      public:
        explicit queueImpl(const char *) {}

        void init() {}

        /**
         * @brief Enqueue element into queue
         */
        bool put(const T &msg)
        {
            q.push(msg);
            return true;
        }

        /**
         * @brief Dequeue into `out`
         * @return false if the queue is empty
         */
        bool get(T &out, uint32_t)
        {
            if (q.empty()) return false;
            out = q.front();
            q.pop();
            return true;
        }

        /**
         * @brief Number of elements currently in the queue
         */
        size_t count() const { return q.size(); }
    };
#else
#error "io::queue requires TARGET_EMBEDDED or TARGET_TEST to be defined"
#endif
} // namespace internal

template <typename T, size_t QUEUE_SIZE> class queue
{
    internal::queueImpl<T, QUEUE_SIZE> queue_impl;
    uint32_t                         overflow_count = 0;

  public:
    explicit queue(const char *name) : queue_impl(name) {}

    /**
     * @brief Create the underlying queue.
     * @note this breaks RAII but embedded moment
     */
    void init() { queue_impl.init(); }

    /**
     * @brief Adds an element to the back of the queue
     * @param msg Element to enqueue, copied into the queue
     * @return Success, or ErrorCode::OUT_OF_RANGE if the queue is ful and the overflow count (get_overflowCount()) is incremented
     */
    [[nodiscard]] result<void> push(const T &msg)
    {
        if (!queue_impl.put(msg)) {
            ++this->overflow_count;
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }
        return {};
    }

    /**
     * @brief Removes the oldest element from the queue, blocking until one is available or the timeout expires
     * @param timeout Max time to wait, defaults to oswaitForever
     * @return The dequued element, or ErrorCode::ERROR if nothing arrived before the bruh timeout 
     * @note In unit tests this never blocks and fails immediately if the queue is empty
     */
    [[nodiscard]] result<T> pop(const uint32_t timeout = std::numeric_limits<uint32_t>::max())
    {
        T msg;
        if (!queue_impl.get(msg, timeout)) {
            return std::unexpected(ErrorCode::ERROR);
        }
        return msg;
    }

    uint32_t get_overflowCount() const { return this->overflow_count; }
    static constexpr size_t get_capacity() { return QUEUE_SIZE; }
    size_t get_count() const { return queue_impl.count(); }
};
} // namespace io
