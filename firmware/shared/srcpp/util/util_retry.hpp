#pragma once
#include <expected>
#include <cstdint>

namespace util
{
// Helper template to extract T and E from std::expected<T, E>
template <typename T> struct expected_traits;

template <typename T, typename E> struct expected_traits<std::expected<T, E>>
{
    using value_type = T;
    using error_type = E;
};

// Overload that deduces T and E from callable return type
template <typename Callable> auto retry(Callable &&k, const uint32_t retry_count) -> decltype(k())
{
    using result_type = decltype(k());
    using traits      = expected_traits<result_type>;
    using T           = traits::value_type;
    using E           = traits::error_type;

    std::expected<T, E> result;
    for (uint32_t i = 0; i < retry_count; i++)
    {
        result = k();
        if (result)
        {
            break;
        }
    }

    return result;
}

} // namespace util