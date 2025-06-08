#include "io_telemMessage.h"
#include "app_utils.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "cmsis_os.h"
#include "io_log.h"
#include "hw_uarts.h"
#include "hw_crc.h"
#include "hw_gpios.h"
#include <assert.h>
#include "io_time.h"

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals

#define QUEUE_SIZE 100
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define MAX_CAN_SIZE CAN_PAYLOAD_BYTES
#define HEADER_SIZE 7
#define MAX_FRAME_SIZE (HEADER_SIZE + 100)
#define MAGIC_HIGH 0xAA
#define MAGIC_LOW 0x55

// TX
static bool               proto_status;
static uint8_t            proto_msg_length;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];
static osMessageQueueId_t message_queue_id;

TelemMessage t_message = TelemMessage_init_zero;

static const osMessageQueueAttr_t queue_attr = {
    .name      = "Telem Message Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

struct MessageBody
{
    uint32_t *values;
    size_t    count;
};

static bool init = false;

static bool telemMessage_appendHeader(uint8_t *frame_buffer, uint8_t *proto_buffer, uint8_t payload_length)
{
    // CRC FUNCTION
    uint32_t crc = hw_crc_calculate((uint32_t *)proto_buffer, (uint32_t)(payload_length));
    // https://stackoverflow.com/questions/39646441/how-to-set-stm32-to-generate-standard-crc32
    crc             = ~crc;
    frame_buffer[0] = MAGIC_HIGH;
    frame_buffer[1] = MAGIC_LOW;
    frame_buffer[2] = payload_length;
    frame_buffer[3] = (uint8_t)((crc >> 24) & 0xFF);
    frame_buffer[4] = (uint8_t)((crc >> 16) & 0xFF);
    frame_buffer[5] = (uint8_t)((crc >> 8) & 0xFF);
    frame_buffer[6] = (uint8_t)(crc & 0xFF);

    memcpy(&frame_buffer[HEADER_SIZE], proto_buffer, payload_length);

    return true;
}

bool encode_message_callback(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    struct MessageBody *ctx = *arg;

    for (size_t i = 0; i < ctx->count; ++i)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_varint(stream, ctx->values[i]))
            return false;
    }

    return true;
}

static bool telemMessage_buildFrameFromRxMsg(const CanMsg *rx_msg, uint8_t *frame_buffer, uint8_t *frame_length)
{
    uint8_t      proto_buffer[QUEUE_SIZE] = { 0 };
    pb_ostream_t stream                   = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

    if (rx_msg->dlc > MAX_CAN_SIZE)
        return false;
    // Fill in fields

    t_message.can_id     = rx_msg->std_id;
    t_message.time_stamp = rx_msg->timestamp;

    uint32_t        size_align = rx_msg->dlc + (4 - rx_msg->dlc % 4);
    static uint32_t body_copy[MAX_CAN_SIZE];
    memcpy(body_copy, rx_msg->data.data32, size_align);

    struct MessageBody encode_ctx = { .values = body_copy, .count = size_align / 4 };

    // Fill in the message data
    t_message.message.funcs.encode = encode_message_callback;
    t_message.message.arg          = &encode_ctx;

    // Encode message into proto_buffer
    proto_status = pb_encode(&stream, TelemMessage_fields, &t_message);
    if (!proto_status)
    {
        LOG_ERROR("Protobuf encoding failed");
        return false;
    }

    proto_msg_length = (uint8_t)stream.bytes_written;
    if (proto_msg_length > QUEUE_SIZE)
    {
        LOG_ERROR("Payload size exceeded maximum allowed size");
        return false;
    }

    // Build frame
    if (!telemMessage_appendHeader(frame_buffer, proto_buffer, proto_msg_length))
    {
        return false;
    }
    *frame_length = HEADER_SIZE + proto_msg_length;
    return true;
}

void io_telemMessage_init()
{
    assert(!init);
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);
    assert(message_queue_id != NULL);
    init = true;
    hw_gpio_writePin(&telem_pwr_en_pin, true);
}

bool io_telemMessage_pushMsgtoQueue(const CanMsg *rx_msg)
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

// process data here
bool io_telemMessage_broadcastMsgFromQueue(void)
{
    CanMsg           queue_out;
    uint8_t          full_frame[MAX_FRAME_SIZE] = { 0 };
    const osStatus_t status                     = osMessageQueueGet(message_queue_id, &queue_out, NULL, osWaitForever);
    assert(status == osOK);

    uint8_t frame_length = 0;
    if (!telemMessage_buildFrameFromRxMsg(&queue_out, full_frame, &frame_length))
    {
        LOG_ERROR("Failed to build frame from received message");
        return false;
    }
    // Start timing for measuring transmission speeds
    bool success = true;
    SEGGER_SYSVIEW_MarkStart(0);
    const ExitCode exit =
        hw_uart_transmit(&_900k_uart, full_frame, frame_length); // send full frame check line 143 for new frame_length
    if (IS_EXIT_OK(exit))
    {
        LOG_INFO("900Mhz Telem Message Sent");
        // print the buffer up to the 27th byte
    }
    else
    {
        LOG_ERROR("900Mhz Telem Message Failed");
    }

    SEGGER_SYSVIEW_MarkStop(0);

    return true;
}
