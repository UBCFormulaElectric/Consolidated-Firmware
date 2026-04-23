#include "util_ringBuffer.hpp"

#include <array>
#include <cstdint>
#include <gtest/gtest.h>

TEST(RingBuffer, EmptyOnConstruction)
{
    util::RingBuffer<uint8_t, 16> rb{};
    EXPECT_EQ(rb.size(), 0u);
    EXPECT_EQ(rb.capacity(), 16u);
    EXPECT_EQ(rb.free(), 16u);
    EXPECT_TRUE(rb.empty());
}

TEST(RingBuffer, PushPeekDiscard)
{
    util::RingBuffer<uint8_t, 8> rb{};
    const std::array<uint8_t, 4> in{{ 1, 2, 3, 4 }};
    ASSERT_TRUE(rb.push(in));
    EXPECT_EQ(rb.size(), 4u);
    EXPECT_EQ(rb.peek(0), 1u);
    EXPECT_EQ(rb.peek(3), 4u);
    rb.discard(2);
    EXPECT_EQ(rb.size(), 2u);
    EXPECT_EQ(rb.peek(0), 3u);
}

TEST(RingBuffer, OverflowReturnsUnexpected)
{
    util::RingBuffer<uint8_t, 4> rb{};
    const std::array<uint8_t, 5> in{{ 1, 2, 3, 4, 5 }};
    EXPECT_FALSE(rb.push(in));
    EXPECT_EQ(rb.size(), 0u);
}

TEST(RingBuffer, WraparoundCopyOut)
{
    util::RingBuffer<uint8_t, 8> rb{};
    const std::array<uint8_t, 6> a{{ 1, 2, 3, 4, 5, 6 }};
    ASSERT_TRUE(rb.push(a));
    rb.discard(5); // head now near the end
    const std::array<uint8_t, 5> b{{ 7, 8, 9, 10, 11 }};
    ASSERT_TRUE(rb.push(b)); // wraps
    EXPECT_EQ(rb.size(), 6u);

    std::array<uint8_t, 6> out{};
    ASSERT_TRUE(rb.copyOut(0, out));
    const std::array<uint8_t, 6> expected{{ 6, 7, 8, 9, 10, 11 }};
    EXPECT_EQ(out, expected);
}

TEST(RingBuffer, CopyOutPastEndFails)
{
    util::RingBuffer<uint8_t, 8> rb{};
    const std::array<uint8_t, 3> in{{ 1, 2, 3 }};
    ASSERT_TRUE(rb.push(in));
    std::array<uint8_t, 4> out{};
    EXPECT_FALSE(rb.copyOut(0, out));
}

TEST(RingBuffer, DiscardClampsToSize)
{
    util::RingBuffer<uint8_t, 4> rb{};
    const std::array<uint8_t, 2> in{{ 1, 2 }};
    ASSERT_TRUE(rb.push(in));
    rb.discard(100);
    EXPECT_EQ(rb.size(), 0u);
}
