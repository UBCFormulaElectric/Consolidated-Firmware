#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "cmsis_os.h"
#include "io_log.h"
#include "hw_uarts.h"

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals

#define CAN_DATA_LENGTH 12
#define UART_LENGTH 1
#define QUEUE_SIZE 50
static bool modem_900_choice;
typedef struct
{
    const UART *modem900M;
    const UART *modem2_4G;
} Modem;
static const Modem modem = { .modem2_4G = &modem2G4_uart, .modem900M = &modem900_uart };
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

void io_telemMessage_init()
{
    modem_900_choice = true; // if false, then using the 2.4GHz,
    message_queue_id = osMessageQueueNew(CAN_DATA_LENGTH, QUEUE_SIZE, &queue_attr);
}

bool io_telemMessage_pushMsgtoQueue(const CanMsg *rx_msg)
{
    uint8_t proto_buffer[QUEUE_SIZE] = { 0 };

    // filter messages, rn for faults and warnings and bms (to verify working when running normally)
    if (rx_msg->std_id != 111 && rx_msg->std_id != 397 && rx_msg->std_id != 205 && rx_msg->std_id != 206 &&
        rx_msg->std_id != 207 && rx_msg->std_id != 208)
    {
        return false;
    }
    // send it over the correct UART functionality
    pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

    // Filling in fields
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
    return true;
}

bool io_telemMessage_broadcastMsgFromQueue(void)
{
    uint8_t    proto_out[QUEUE_SIZE] = { 0 };
    uint8_t    zero_test             = 0;
    osStatus_t status                = osMessageQueueGet(message_queue_id, &proto_out, NULL, osWaitForever);
    proto_out_length                 = proto_out[49];
    proto_out[49]                    = 0;

    // Start timing for measuring transmission speeds
    SEGGER_SYSVIEW_MarkStart(0);
    if (modem_900_choice)
    {
        hw_uart_transmitPoll(modem.modem900M, &proto_out_length, UART_LENGTH, UART_LENGTH);
        hw_uart_transmitPoll(modem.modem900M, proto_out, (uint8_t)sizeof(proto_out), 100);
        // hw_uart_transmitPoll(modem->modem900M, &zero_test, UART_LENGT/H, UART_LENGTH);
    }
    else
    {
        hw_uart_transmitPoll(modem.modem2_4G, &proto_msg_length, UART_LENGTH, UART_LENGTH);
        hw_uart_transmitPoll(modem.modem2_4G, proto_out, (uint8_t)sizeof(proto_out), 100);
    }
    SEGGER_SYSVIEW_MarkStop(0);

    return true;
}
