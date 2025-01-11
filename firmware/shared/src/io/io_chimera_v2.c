#include <stdlib.h>
#include "hw_usb.h"


/*
    CHIMERA Packet Format:
    [Non-zero indicator][ low length byte ][ high length byte ][... content bytes * length ...]
*/

void io_chimera_main(uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len), uint32_t net_name_gpio, uint32_t net_name_adc) {

    // init usb peripheral.
    hw_usb_init(transmit_handle);

    // dump the queue.
    for (int i = 0; true; i += 1)
    {
        if (hw_usb_recieve() == 0x0) continue;

        // Length-delimination sent with little-endian.
        uint8_t low_byte = hw_usb_recieve();
        uint8_t high_byte = hw_usb_recieve();

        // Extract length.
        uint16_t length = low_byte + high_byte << 8;

        // Populate buffer.
        uint8_t *buf = malloc(length * sizeof(uint8_t));
        for (int i = 0; i < length; i += 1) {
            buf[i] = hw_usb_recieve();
        }

        // TODO: PROCESS BUFFER

        free(buf);

    }
};

