#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>

#include "util_errorCodes.hpp"

namespace util
{
template <typename T, std::size_t N> class RingBuffer
{
    static_assert(N > 0, "RingBuffer capacity must be > 0");

    std::array<T, N> buf_{};
    std::size_t      head_ = 0;
    std::size_t      tail_ = 0;
    std::size_t      size_ = 0;

  public:
    [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return N; }
    [[nodiscard]] constexpr std::size_t free() const noexcept { return N - size_; }
    [[nodiscard]] constexpr bool        empty() const noexcept { return size_ == 0; }

    [[nodiscard]] std::expected<void, ErrorCode> push(std::span<const T> src) noexcept
    {
        if (src.size() > free())
            return std::unexpected(ErrorCode::ERROR);

        for (const T &v : src)
        {
            buf_[tail_] = v;
            tail_       = (tail_ + 1) % N;
        }
        size_ += src.size();
        return {};
    }

    // Read T at logical offset from head without consuming.
    [[nodiscard]] T peek(std::size_t offset) const
    {
        return buf_[(head_ + offset) % N];
    }

    // Copy out a contiguous logical run starting at offset, without consuming.
    [[nodiscard]] std::expected<void, ErrorCode> copyOut(std::size_t offset, std::span<T> dst) const noexcept
    {
        if (offset + dst.size() > size_)
            return std::unexpected(ErrorCode::ERROR);

        for (std::size_t i = 0; i < dst.size(); ++i)
            dst[i] = buf_[(head_ + offset + i) % N];
        return {};
    }

    void discard(std::size_t n) noexcept
    {
        if (n > size_)
            n = size_;
        head_ = (head_ + n) % N;
        size_ -= n;
    }

    void clear() noexcept
    {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }
};
} // namespace util
