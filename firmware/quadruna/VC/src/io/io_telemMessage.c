#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "io_time.h"
#include "io_log.h"
// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such
// Private Globals
static bool   modem_900_choice;
static Modem *modem;
#define CAN_DATA_LENGTH 8
#define UART_LENGTH 1
#define QUEUE_SIZE 12
#define QUEUE_BYTES 4 * QUEUE_SIZE // this is all temp
static bool    proto_status;
static uint8_t proto_msg_length;
static uint8_t proto_buffer[QUEUE_SIZE]; // TODO: verify that this is the needed size (most likely can be smaller)
TelemMessage   t_message = TelemMessage_init_zero;
void           io_telemMessage_init(Modem *m)
{
    modem_900_choice = true; // if false, then using the 2.4GHz,
    modem            = m;
}
bool io_telemMessage_broadcast(CanMsg *rx_msg)
{
    // send it over the correct UART functionality
    pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));
    // filling in fields
    t_message.can_id = 130; //(int32_t)(rx_msg->std_id);
    // for (uint8_t i = 0; i < CAN_DATA_LENGTH; i++)
    // {
    //     t_message.message[i] = rx_msg->data[i];
    // }
    t_message.message    = 8;
    t_message.time_stamp = (int32_t)io_time_getCurrentMs();
    // encoding message
    proto_status     = pb_encode(&stream, TelemMessage_fields, &t_message);
    proto_msg_length = (uint8_t)stream.bytes_written;
    // LOG_INFO("%d, %d", t_message.can_id, t_message.time_stamp);
    // LOG_INFO("%d %d %d %d %d %d %d %d", t_message.message[0], t_message.message[1], t_message.message[2],
    //     t_message.message[3], t_message.message[4], t_message.message[5], t_message.message[6],
    //     t_message.message[7]);
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