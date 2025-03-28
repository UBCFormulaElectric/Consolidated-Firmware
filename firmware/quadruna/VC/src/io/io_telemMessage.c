#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "cmsis_os.h"
#include "io_log.h"
#include "hw_uarts.h"
#include "hw_crc.h"
#include <assert.h>

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals

#define CAN_DATA_LENGTH 12
#define UART_LENGTH 1
#define QUEUE_SIZE 52

#define HEADER_SIZE 7
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE
#define MAX_FRAME_SIZE (HEADER_SIZE + QUEUE_BYTES)
#define MAGIC_HIGH 0xAA
#define MAGIC_LOW 0x55

static bool modem_900_choice;
typedef struct
{
    const UART *modem900M;
    const UART *modem2_4G;
} Modem;
static const Modem modem = { .modem2_4G = &modem2G4_uart, .modem900M = &modem900_uart };

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

static bool init = false;

static bool telemMessage_appendHeader(uint8_t *frame_buffer, uint8_t *proto_buffer, uint8_t payload_length)
{
    // CRC FUNCTION
    uint32_t crc = hw_crc_calculate((uint32_t *)proto_buffer, (uint32_t)(payload_length / sizeof(uint32_t)));

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

static bool telemMessage_buildFrameFromRxMsg(const CanMsg *rx_msg, uint8_t *frame_buffer, uint8_t *frame_length)
{
    uint8_t      proto_buffer[QUEUE_SIZE] = { 0 };
    pb_ostream_t stream                   = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

    if (rx_msg->dlc > 8)
        return false;
    t_message.can_id     = (int32_t)(rx_msg->std_id);
    t_message.message_0  = rx_msg->data[0];
    t_message.message_1  = rx_msg->data[1];
    t_message.message_2  = rx_msg->data[2];
    t_message.message_3  = rx_msg->data[3];
    t_message.message_4  = rx_msg->data[4];
    t_message.message_5  = rx_msg->data[5];
    t_message.message_6  = rx_msg->data[6];
    t_message.message_7  = rx_msg->data[7];
    t_message.time_stamp = (int32_t)rx_msg->timestamp;

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

    // padding required for crc function to not have concat issues
    uint8_t padded_length = (uint8_t)((proto_msg_length + 3u) & ~3u);
    if (padded_length > proto_msg_length)
    {
        memset(&proto_buffer[proto_msg_length], 0, padded_length - proto_msg_length);
    }

    // Build frame
    if (!telemMessage_appendHeader(frame_buffer, proto_buffer, padded_length))
    {
        return false;
    }
    *frame_length = HEADER_SIZE + padded_length;
    return true;
}

void io_telemMessage_init()
{
    assert(!init);
    modem_900_choice = true; // if false, then using the 2.4GHz,
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);
    assert(message_queue_id != NULL);
    init = true;
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
            LOG_WARN("Telem Queue Overflow, count: %d", telem_overflow_count);
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
    if (modem_900_choice)
    {
        success &= hw_uart_transmitPoll(
            modem.modem900M, full_frame, frame_length, 100); // send full frame check line 143 for new frame_length
    }
    else
    {
        LOG_ERROR("NOT SUPPORTED");
    }
    SEGGER_SYSVIEW_MarkStop(0);

    return true;
}