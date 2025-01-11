#include "hw_usb.h"

#include <stdlib.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include "io_log.h"


/*
    CHIMERA Packet Format:
    [  Non-zero Byte   ][ length low byte  ][ length high byte ][  content bytes   ]...
*/

void io_chimera_main(
    uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len),
    uint32_t net_name_gpio,
    uint32_t net_name_adc)
{
    // init usb peripheral.
    hw_usb_init(transmit_handle);

    // dump the queue.
    for (int i = 0; true; i += 1)
    {
        if (hw_usb_recieve() == 0x0)
            continue;

        // Length-delimination sent with little-endian.
        uint8_t low_byte  = hw_usb_recieve();
        uint8_t high_byte = hw_usb_recieve();

        // Extract length.
        uint16_t length = low_byte + high_byte << 8;

        // Populate buffer.
        uint8_t *buf = malloc(length * sizeof(uint8_t));
        for (int i = 0; i < length; i += 1)
            buf[i] = hw_usb_recieve();

        io_chimera_handleBuf(buf, length);
        free(buf);
    }
};

void io_chimera_handleBuf(uint8_t *buf, uint16_t length) {

    // Receive message.
    DebugMessage message = DebugMessage_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buf, length);
    bool status = pb_decode(&stream, DebugMessage_fields, &message);
    if (!status) {
        LOG_ERROR("Error decoding chimera message stream");
        return;
    }

    // TODO: Handle debug message.

}