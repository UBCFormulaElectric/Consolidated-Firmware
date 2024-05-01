#include "io_telemMessage.h"
#include "telem.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "cmsis_os.h"
#include "queue.h"

// create the truth table for now to decide which amount of things to use
// create or grab the constants for the different modem and pins and such

// Private Globals
static bool   modem_900_choice;
static Modem *modem;

#define QUEUE_SIZE 12
#define QUEUE_BYTES 4 * QUEUE_SIZE // this is all temp

static bool    proto_status;
static uint8_t proto_msg_length;
static uint8_t proto_buffer[QUEUE_SIZE]; // TODO: verify that this is the needed size (most likely can be smaller)
TelemMessage   message = TelemMessage_init_zero;

void io_telemMessage_init(Modem *m)
{
    modem_900_choice = true; // if false, then using the 2.4GHz,
    modem            = m;
}

//make a converter file 

bool io_telemMessage_broadcast()
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

    if (modem_900_choice == true)
    {
        hw_uart_transmitPoll(modem->modem900M, &proto_msg_length, 1, 1);
        hw_uart_transmitPoll(modem->modem900M, proto_buffer, (uint8_t)sizeof(proto_buffer), 1);
    }
    else
    {
        hw_uart_transmitPoll(modem->modem2_4G, &proto_msg_length, 1, 1);
        hw_uart_transmitPoll(modem->modem2_4G, proto_buffer, (uint8_t)sizeof(proto_buffer), 1); //TODO: get rid of the magic numbers
    }
    return true;
}