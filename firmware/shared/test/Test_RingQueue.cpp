#include <gtest/gtest.h>

extern "C"
{
#include "App_RingQueue.h"
}

TEST(RingQueueTest, test_push_and_pop)
{
    RingQueue queue;
    App_RingQueue_Init(&queue, 10);

    // Queue is empty, no data
    uint8_t popped;
    ASSERT_FALSE(App_RingQueue_Pop(&queue, &popped));

    // Push 1, 2, 3
    App_RingQueue_Push(&queue, 1);
    App_RingQueue_Push(&queue, 2);
    App_RingQueue_Push(&queue, 3);

    // Pop 1, 2, 3
    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 1);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 2);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 3);

    // Queue is empty, no data
    ASSERT_FALSE(App_RingQueue_Pop(&queue, &popped));

    // Push 4, then pop 4
    App_RingQueue_Push(&queue, 4);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 4);

    // Queue is empty, no data
    ASSERT_FALSE(App_RingQueue_Pop(&queue, &popped));
}

TEST(RingQueueTest, test_buffer_overflow)
{
    RingQueue queue;
    App_RingQueue_Init(&queue, 3);

    uint8_t popped;
    ASSERT_FALSE(App_RingQueue_Pop(&queue, &popped));

    // Push 1, 2, 3, 4
    App_RingQueue_Push(&queue, 1);
    App_RingQueue_Push(&queue, 2);
    App_RingQueue_Push(&queue, 3);
    App_RingQueue_Push(&queue, 4);

    // Pop 2, 3, 4 (queue overflowed, 1 was overwritten)
    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 2);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 3);

    // Push more elements
    App_RingQueue_Push(&queue, 5);
    App_RingQueue_Push(&queue, 6);
    App_RingQueue_Push(&queue, 7);
    App_RingQueue_Push(&queue, 8);
    App_RingQueue_Push(&queue, 9);
    App_RingQueue_Push(&queue, 10);

    // Pop 3 most recent elements (rest were overwritten)
    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 8);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 9);

    ASSERT_TRUE(App_RingQueue_Pop(&queue, &popped));
    ASSERT_EQ(popped, 10);

    // Queue is empty, no data
    ASSERT_FALSE(App_RingQueue_Pop(&queue, &popped));
}
