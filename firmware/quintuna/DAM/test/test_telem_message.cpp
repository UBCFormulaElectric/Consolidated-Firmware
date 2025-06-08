#include "test_DAMBase.hpp"

// not really a test but just a place to play with the proto buf decode and encode
#define QUEUE_SIZE 100
uint8_t proto_buffer[QUEUE_SIZE] = { 0 };
extern "C"
{
#include "io_canMsg.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include "io_telemMessage.h"
#define QUEUE_BYTES sizeof(CanMsg) * QUEUE_SIZE

#define HEADER_SIZE 7
#define MAX_FRAME_SIZE (HEADER_SIZE + 100)
#define MAGIC_HIGH 0xAA
#define MAGIC_LOW 0x55

    static bool    proto_status;
    static uint8_t proto_msg_length;

    TelemMessage t_message = TelemMessage_init_zero;

    struct MessageBody
    {
        uint32_t *values;
        size_t    count;
    };

    static bool telemMessage_appendHeader(uint8_t *frame_buffer, uint8_t *proto_buffer, uint8_t payload_length)
    {
        // CRC FUNCTION
        uint32_t crc = 0;
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
        struct MessageBody *ctx = static_cast<MessageBody *>(*arg);

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
        pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

        if (rx_msg->dlc > 64)
            return false;
        // Fill in fields

        t_message.can_id     = rx_msg->std_id;
        t_message.time_stamp = rx_msg->timestamp;

        uint32_t        size_align = rx_msg->dlc + (4 - rx_msg->dlc % 4);
        static uint32_t body_copy[64];
        memcpy(body_copy, rx_msg->data.data32, size_align);
        // memmove()

        struct MessageBody encode_ctx = { .values = body_copy, .count = size_align / 4 };

        // Fill in the message data
        t_message.message.funcs.encode = encode_message_callback;
        t_message.message.arg          = &encode_ctx;

        // Encode message into proto_buffer
        proto_status = pb_encode(&stream, TelemMessage_fields, &t_message);
        if (!proto_status)
        {
            return false;
        }

        proto_msg_length = (uint8_t)stream.bytes_written;
        if (proto_msg_length > QUEUE_SIZE)
        {
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

    void io_telemMessage_init() {}

    bool io_telemMessage_pushMsgtoQueue(const CanMsg *rx_msg)
    {
        uint8_t full_frame[MAX_FRAME_SIZE] = { 0 };
        uint8_t frame_length               = 0;
        if (!telemMessage_buildFrameFromRxMsg(rx_msg, full_frame, &frame_length))
        {
            return false;
        }
        return true;
    }

    // process data here
    bool io_telemMessage_broadcastMsgFromQueue(void)
    {
        return true;
    }
}

class DAMTelemTest : public DAMBaseTest
{
};

// Temporary buffer to hold decoded values
uint32_t decoded_values[64 / 4]; // worst-case size
size_t   decoded_count = 0;
bool     decode_message_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint32_t *out = reinterpret_cast<uint32_t *>(*arg);

    while (stream->bytes_left)
    {
        uint64_t val;
        if (!pb_decode_varint(stream, &val))
            return false;

        out[decoded_count++] = static_cast<uint32_t>(val);
    }
    return true;
};

TEST_F(DAMTelemTest, telem_decode_test)
{
    CanMsg  msg = {};
    uint8_t data[64];
    for (size_t i = 0; i < 64; i++)
    {
        data[i] = i;
    }
    memcpy(msg.data.data8, data, 64);

    msg.dlc    = 33;
    msg.std_id = 111;
    io_telemMessage_pushMsgtoQueue(&msg);

    // Decode the proto_buffer after it's filled by io_telemMessage_pushMsgtoQueue

    TelemMessage decoded_msg = TelemMessage_init_zero;

    decoded_msg.message.funcs.decode = decode_message_callback;
    decoded_msg.message.arg          = decoded_values;

    pb_istream_t istream = pb_istream_from_buffer(proto_buffer, proto_msg_length);

    bool decode_ok = pb_decode(&istream, TelemMessage_fields, &decoded_msg);

    ASSERT_TRUE(decode_ok);
    EXPECT_EQ(decoded_msg.can_id, msg.std_id);
    EXPECT_EQ(decoded_msg.time_stamp, msg.timestamp);

    // Print or assert decoded values
    uint8_t *decoded_bytes = reinterpret_cast<uint8_t *>(decoded_values);
    for (size_t i = 0; i < msg.dlc; ++i)
    {
        EXPECT_EQ(decoded_bytes[i], msg.data.data8[i]);
    }
}