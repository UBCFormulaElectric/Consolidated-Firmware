#include "Test_Shared.h"

extern "C"
{
#include "App_RingQueue.h"
}

static const sensor_msg_config *config;

static const sensor_msg_config msg_config = { .ring_queue_overflow_callback =
                                                  App_CanAlerts_DCM_Warning_RingQueueOverflow_Set };

TEST(RingQueueTest, test_push_and_pop)
{
    App_RingQueue_Init(&msg_config);

    // Queue is empty, no data
    uint8_t popped;
    ASSERT_FALSE(App_RingQueue_Pop(&popped));

    // Push 1, 2, 3
    uint8_t data1 = 1;
    uint8_t data1 = 2;
    uint8_t data1 = 3;

    App_RingQueue_Push(&data1);
    App_RingQueue_Push(&data2);
    App_RingQueue_Push(&data3);

    // Pop 1, 2, 3
    ASSERT_TRUE(App_RingQueue_Pop(&popped));
    ASSERT_EQ(popped, 1);

    ASSERT_TRUE(App_RingQueue_Pop(&popped));
    ASSERT_EQ(popped, 2);

    ASSERT_TRUE(App_RingQueue_Pop(&popped));
    ASSERT_EQ(popped, 3);

    // Queue is empty, no data
    ASSERT_FALSE(App_RingQueue_Pop(&popped));

    // Push 4, then pop 4
    App_RingQueue_Push(&queue, 4);

    ASSERT_TRUE(App_RingQueue_Pop(&popped));
    ASSERT_EQ(popped, 4);

    // Queue is empty, no data
    ASSERT_FALSE(App_RingQueue_Pop(&popped));
}
