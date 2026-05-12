#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <utility>

#include "util_errorCodes.hpp"

namespace util
{
// Single-producer, single-consumer lock-free ring buffer.
//
// Producer-only methods: push, free.
// Consumer-only methods: peek, peekU32Le, contiguousRange, copyOut, discard.
// Either-side methods:   size, capacity, empty.
//
// head_ and tail_ are monotonic counters that never wrap modulo N; the modulo
// is applied only at the access site. Unsigned subtraction yields the correct
// live span as long as that span <= N, which the API guarantees.
template <typename T, std::size_t N> class RingBuffer
{
    static_assert(N > 0, "RingBuffer capacity must be > 0");

    std::array<T, N>         buf_{};
    std::atomic<std::size_t> head_{ 0 }; // consumer writes (release), producer reads (acquire)
    std::atomic<std::size_t> tail_{ 0 }; // producer writes (release), consumer reads (acquire)

  public:
    [[nodiscard]] std::size_t size() const noexcept
    {
        const std::size_t t = tail_.load(std::memory_order_acquire);
        const std::size_t h = head_.load(std::memory_order_acquire);
        return t - h;
    }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return N; }
    [[nodiscard]] std::size_t           free() const noexcept { return N - size(); }
    [[nodiscard]] bool                  empty() const noexcept { return size() == 0; }

    // Producer-only. Writes bytes then publishes via release-store of tail_,
    // establishing happens-before with the consumer's acquire-load.
    [[nodiscard]] std::expected<void, ErrorCode> push(std::span<const T> src) noexcept
    {
        const std::size_t t = tail_.load(std::memory_order_relaxed);
        const std::size_t h = head_.load(std::memory_order_acquire);
        if (src.size() > N - (t - h))
            return std::unexpected(ErrorCode::ERROR);

        for (std::size_t i = 0; i < src.size(); ++i)
            buf_[(t + i) % N] = src[i];

        tail_.store(t + src.size(), std::memory_order_release);
        return {};
    }

    // Consumer-only. Read T at logical offset from head without consuming.
    [[nodiscard]] T peek(std::size_t offset) const
    {
        const std::size_t h = head_.load(std::memory_order_relaxed);
        return buf_[(h + offset) % N];
    }

    // Consumer-only. Read a little-endian uint32_t at logical offset from head without consuming.
    [[nodiscard]] uint32_t peekU32Le(std::size_t offset) const noexcept
    {
        static_assert(sizeof(T) == 1, "peekU32Le requires byte-sized element type");

        uint32_t v = 0;
        v |= static_cast<uint32_t>(peek(offset + 0)) << 0;
        v |= static_cast<uint32_t>(peek(offset + 1)) << 8;
        v |= static_cast<uint32_t>(peek(offset + 2)) << 16;
        v |= static_cast<uint32_t>(peek(offset + 3)) << 24;
        return v;
    }

    // Consumer-only. Returns up to two contiguous spans covering the logical range
    // [offset, offset+len). Second span is empty if the range does not wrap. On
    // out-of-bounds, both spans are empty.
    [[nodiscard]] std::pair<std::span<const T>, std::span<const T>>
        contiguousRange(std::size_t offset, std::size_t len) const noexcept
    {
        if (offset + len > size())
            return { {}, {} };

        const std::size_t h     = head_.load(std::memory_order_relaxed);
        const std::size_t start = (h + offset) % N;
        if (start + len <= N)
            return { std::span<const T>{ buf_.data() + start, len }, std::span<const T>{} };

        const std::size_t first = N - start;
        return { std::span<const T>{ buf_.data() + start, first }, std::span<const T>{ buf_.data(), len - first } };
    }

    // Consumer-only. Copy out a contiguous logical run starting at offset, without consuming.
    [[nodiscard]] std::expected<void, ErrorCode> copyOut(std::size_t offset, std::span<T> dst) const noexcept
    {
        if (offset + dst.size() > size())
            return std::unexpected(ErrorCode::ERROR);

        const std::size_t h = head_.load(std::memory_order_relaxed);
        for (std::size_t i = 0; i < dst.size(); ++i)
            dst[i] = buf_[(h + offset + i) % N];
        return {};
    }

    // Consumer-only.
    void discard(std::size_t n) noexcept
    {
        const std::size_t s = size();
        if (n > s)
            n = s;
        const std::size_t h = head_.load(std::memory_order_relaxed);
        head_.store(h + n, std::memory_order_release);
    }

    // NOT SPSC-safe. Only call when both producer and consumer are quiescent.
    void clear() noexcept
    {
        head_.store(0, std::memory_order_relaxed);
        tail_.store(0, std::memory_order_relaxed);
    }
};
} // namespace util
