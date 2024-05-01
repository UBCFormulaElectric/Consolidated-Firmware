#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "cmsis_os.h"
#include "queue.h"

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such

// Private Globals
static bool Modem900;

#define QUEUE_SIZE 12
#define QUEUE_BYTES 4 * QUEUE_SIZE // this is all temp

static bool    proto_status;
static uint8_t proto_msg_length;
static uint8_t proto_buffer[QUEUE_SIZE]; // TODO: verify that this is the needed size (most likely can be smaller)
TelemMessage   message = TelemMessage_init_zero;

void io_telemMessage_init()
{
    Modem900 = true; // if false, then using the 2.4GHz, not implemented now bc no 2.4 yet
}

void io_telemMessage_broadcast(UART *modem)
{
    // send it over the correct UART functionality
    pb_ostream_t stream = pb_ostream_from_buffer(proto_buffer, sizeof(proto_buffer));

    // filling in fields
    message.can_id     = 53;
    message.data       = 23;
    message.time_stamp = 9;

    // encoding message
    proto_status     = pb_encode(&stream, TelemMessage_fields, &message);
    proto_msg_length = (uint8_t)stream.bytes_written;
    hw_uart_transmitPoll(modem, &proto_msg_length, 1, 1);
    hw_uart_transmitPoll(modem, proto_buffer, (uint8_t)sizeof(proto_buffer), 1); // fun string

    return;
}