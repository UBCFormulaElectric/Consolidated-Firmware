#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "io_time.h"
#include "cmsis_os.h"
#include "queue.h"
#include "io_log.h"

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals
static bool   modem_900_choice;
static Modem *modem;
#define CAN_DATA_LENGTH 8
#define UART_LENGTH 1
#define QUEUE_SIZE 12
#define QUEUE_BYTES CAN_DATA_LENGTH *QUEUE_SIZE

static bool proto_status;
// static uint8_t proto_msg_length;
static uint8_t            proto_buffer[QUEUE_SIZE];
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

void io_telemMessage_init(Modem *m)
{
    modem_900_choice = true; // if false, then using the 2.4GHz,
    modem            = m;
    message_queue_id = osMessageQueueNew(QUEUE_SIZE, sizeof(CanMsg), &queue_attr);
}

bool io_telemMessage_pushMsgtoQueue(CanMsg *rx_msg)
{
    // filter messages
    if (rx_msg->std_id != 130)
    {
        return false;
    }
    // send it over the correct UART functionality
    pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));
    // filling in fields
    t_message.can_id     = (int32_t)(rx_msg->std_id);
    t_message.message_0  = rx_msg->data[0];
    t_message.message_1  = rx_msg->data[1];
    t_message.message_2  = rx_msg->data[2];
    t_message.message_3  = rx_msg->data[3];
    t_message.message_4  = rx_msg->data[4];
    t_message.message_5  = rx_msg->data[5];
    t_message.message_6  = rx_msg->data[6];
    t_message.message_7  = rx_msg->data[7];
    t_message.time_stamp = (int32_t)io_time_getCurrentMs();
    // encoding message
    proto_status = pb_encode(&stream, TelemMessage_fields, &t_message);
    // proto_msg_length = (uint8_t)stream.bytes_written;
    if (osMessageQueuePut(message_queue_id, &proto_buffer, 0, 0) != osOK)
    {
        return true;
    }
    return false;
}

bool io_telemMessage_broadcastMsgFromQueue(void)
{
    static uint8_t proto_out[QUEUE_SIZE];
    static uint8_t proto_msg_length = 9;
    osMessageQueueGet(message_queue_id, &proto_out, NULL, osWaitForever);
    LOG_INFO("proto popped and on to uart");
    if (modem_900_choice == true)
    {
        hw_uart_transmitPoll(modem->modem900M, &proto_msg_length, UART_LENGTH, 100);
        hw_uart_transmitPoll(modem->modem900M, proto_buffer, (uint8_t)sizeof(proto_buffer), 100);
    }
    else
    {
        hw_uart_transmitPoll(modem->modem2_4G, &proto_msg_length, UART_LENGTH, UART_LENGTH);
        hw_uart_transmitPoll(modem->modem2_4G, proto_buffer, (uint8_t)sizeof(proto_buffer), UART_LENGTH);
    }
    return true;
}
