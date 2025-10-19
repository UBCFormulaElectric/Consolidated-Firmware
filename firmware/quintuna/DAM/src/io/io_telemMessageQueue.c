#include "io_telemMessageQueue.h"

#include "cmsis_os.h"

#include <stdint.h>
#include <assert.h>

#include "io_log.h"

#define QUEUE_SIZE 1000 /* Was 52 before */
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE

// TX
static StaticQueue_t              queue_control_block;
static uint8_t                    queue_buf[QUEUE_BYTES];
static osMessageQueueId_t         message_queue_id;
static const osMessageQueueAttr_t queue_attr = {
    .name      = "Telem Message TX Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

// RX
// static uint8_t  rx_buffer[MAX_FRAME_SIZE];
// static uint32_t rx_buffer_pos     = 0;
// static bool     message_available = false;

static bool init = false;

void io_telemMessageQueue_init(void)
{
    assert(!init);
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);
    assert(message_queue_id != NULL);

    init = true;
}

bool io_telemMessageQueue_pushTx(const CanMsg *rx_msg)
{
    assert(init);

    static uint32_t telem_overflow_count = 0;
    // store the result of the message queue put.

    const osStatus_t s = osMessageQueuePut(
        message_queue_id, rx_msg, 0U, 0U); // status of queue put EVERYTHING BUT THIS SHOULD BE IN A HELPER

    if (s != osOK)
    {
        if (s == osErrorResource)
        {
            telem_overflow_count++;
            // LOG_WARN("Telem Queue Overflow, count: %d", telem_overflow_count);
        }
        else
        {
            LOG_WARN("queue problem %d", s);
        }
    }
    return true;
}

CanMsg io_telemMessageQueue_popTx(void)
{
    assert(init);

    CanMsg           queue_out;
    const osStatus_t status = osMessageQueueGet(message_queue_id, &queue_out, NULL, osWaitForever);
    assert(status == osOK);
    return queue_out;
}
