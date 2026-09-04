#pragma once

#include <array>
#include <cassert>
#include <cstddef>

#include "io_queue.hpp"

namespace app
{
template <typename T, size_t DEPTH, size_t MAX_SUBSCRIBERS> class Channel
{
  public:
    using Subscription = io::queue<T, DEPTH>;

    void subscribe(Subscription &subscription)
    {
        assert(subscriber_count < MAX_SUBSCRIBERS);
        subscription.init();
        subscribers[subscriber_count] = &subscription;
        subscriber_count++
    }

    void publish(const T &value) const
    {
        for (size_t i = 0; i < subscriber_count; i++)
            (void)subscribers[i]->push(value);
    }

  private:
    std::array<Subscription *, MAX_SUBSCRIBERS> subscribers{};
    size_t                                      subscriber_count = 0;
};
} // namespace app
