#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "io_time.h"
#include "cmsis_os.h"
#include "queue.h"
#include "io_log.h"
#include "app_canDataCapture.h"

// Private Globals

#define CAN_DATA_LENGTH 12
#define UART_LENGTH 1
#define QUEUE_SIZE 50
static bool         modem_900_choice;
static const Modem *modem = NULL;
#define QUEUE_BYTES CAN_DATA_LENGTH *QUEUE_SIZE

static bool               proto_status;
static uint8_t            proto_msg_length;
static StaticQueue_t      queue_control_block;
static uint8_t            queue_buf[QUEUE_BYTES];
static osMessageQueueId_t message_queue_id;
static uint8_t            proto_out_length;

TelemMessage t_message = TelemMessage_init_zero;

static const osMessageQueueAttr_t queue_attr = {
    .name      = "Telem Message Queue",
    .attr_bits = 0,
    .cb_mem    = &queue_control_block,
    .cb_size   = sizeof(StaticQueue_t),
    .mq_mem    = queue_buf,
    .mq_size   = QUEUE_BYTES,
};

void io_telemMessage_init(const Modem *m)
{
    modem_900_choice = true; // if false, then using the 2.4GHz,
    modem            = m;
    message_queue_id = osMessageQueueNew(CAN_DATA_LENGTH, QUEUE_SIZE, &queue_attr);
}

bool io_telemMessage_pushMsgtoQueue(CanMsg *msg)
{
    uint8_t proto_buffer[QUEUE_SIZE] = { 0 };

    // filter messages

    uint32_t msg_id = msg->std_id;
    uint32_t time_ms = io_time_getCurrentMs();
    if (app_dataCapture_needsTelem((uint16_t)msg_id, time_ms)){ //TODO: proper app / io seperation
        // start protobuf
        pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

        // filling in fields
        if (msg->dlc > 8)
            return false;
        t_message.can_id    = (int32_t)msg_id;
        t_message.message_0 = msg->data[0];
        t_message.message_1 = msg->data[1];
        t_message.message_2 = msg->data[2];
        t_message.message_3 = msg->data[3];
        t_message.message_4 = msg->data[4];
        t_message.message_5 = msg->data[5];
        t_message.message_6 = msg->data[6];
        t_message.message_7 = msg->data[7];

        // encoding message

        proto_status                         = pb_encode(&stream, TelemMessage_fields, &t_message);
        proto_msg_length                     = (uint8_t)stream.bytes_written;
        proto_buffer[49]                     = proto_msg_length;
        static uint32_t telem_overflow_count = 0;
        osStatus_t      s                    = osMessageQueuePut(message_queue_id, &proto_buffer, 0U, 0U);
        if (s != osOK)
        {
            telem_overflow_count++;
            LOG_WARN("queue problem");
        }
        else
        {
            LOG_INFO("proto pushed to queue");
        }
        return true;
    }
    else{
        return false;
    }
}

bool io_telemMessage_broadcastMsgFromQueue(void)
{
    uint8_t    proto_out[QUEUE_SIZE] = { 0 };
    uint8_t    zero_test             = 0;
    osStatus_t status                = osMessageQueueGet(message_queue_id, &proto_out, NULL, osWaitForever);
    proto_out_length                 = proto_out[49];
    proto_out[49]                    = 0;

    LOG_INFO("proto popped and on to uart");
    if (modem_900_choice)
    {
        hw_uart_transmitPoll(modem->modem900M, &proto_out_length, UART_LENGTH, UART_LENGTH);
        hw_uart_transmitPoll(modem->modem900M, proto_out, (uint8_t)sizeof(proto_out), 100);
        // hw_uart_transmitPoll(modem->modem900M, &zero_test, UART_LENGT/H, UART_LENGTH);
    }
    else
    {
        hw_uart_transmitPoll(modem->modem2_4G, &proto_msg_length, UART_LENGTH, UART_LENGTH);
        hw_uart_transmitPoll(modem->modem2_4G, proto_out, (uint8_t)sizeof(proto_out), 100);
    }
    return true;
}
