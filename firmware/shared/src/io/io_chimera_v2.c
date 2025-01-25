#include "hw_usb.h"
#include <stdlib.h>
#include "io_log.h"
#include <assert.h>

// Protobuf.
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

// Peripheral interfaces.
#include "hw_adc.h"
#include "hw_gpio.h"

static Gpio       **id_to_gpio;
static AdcChannel **id_to_adc;

static const Gpio *io_chimera_v2_parseNetLabelGpio(const GpioNetName *net_name)
{
    switch (net_name->which_name)
    {
        case GpioNetName_f4dev_net_name_tag:
        {
            return id_to_gpio[net_name->name.f4dev_net_name];
        }
        default:
        {
            return 0U;
        }
    }
}

static const AdcChannel *io_chimera_v2_parseNetLabelAdc(const AdcNetName *net_name)
{
    switch (net_name->which_name)
    {
        case AdcNetName_f4dev_net_name_tag:
        {
            return id_to_adc[net_name->name.f4dev_net_name];
        }
        default:
        {
            return NULL;
        }
    }
}

void io_chimera_v2_handleContent(uint8_t *content, uint16_t length, uint32_t net_name_gpio, uint32_t net_name_adc)
{
    // Receive message.
    DebugMessage msg            = DebugMessage_init_zero;
    pb_istream_t content_stream = pb_istream_from_buffer(content, length);
    if (!pb_decode(&content_stream, DebugMessage_fields, &msg))
    {
        LOG_ERROR("Error decoding chimera message stream");
        return;
    }

    switch (msg.which_payload)
    {
        // GPIO read message.
        case DebugMessage_gpio_read_tag:
        {
            const Gpio *gpio = io_chimera_v2_parseNetLabelGpio(&msg.payload.gpio_read.net_name);

            // Add one for enum scale offset.
            msg.payload.gpio_read.value = hw_gpio_readPin(gpio) + 1;
            break;
        }
        // GPIO write message.
        case DebugMessage_gpio_write_tag:
        {
            const Gpio *gpio = io_chimera_v2_parseNetLabelGpio(&msg.payload.gpio_write.net_name);

            // Subtract one for enum scale offset.
            hw_gpio_writePin(gpio, msg.payload.gpio_write.value - 1);
            break;
        }
        // ADC read message.
        case DebugMessage_adc_tag:
        {
            const AdcChannel *adc_channel = io_chimera_v2_parseNetLabelAdc(&msg.payload.adc.net_name);
            msg.payload.adc.value         = hw_adc_getVoltage(adc_channel);
            break;
        }
        default:
            break;
    }

    // Encode protobuf to bytes.
    pb_byte_t    out_buffer[10000];
    pb_ostream_t out_stream = pb_ostream_from_buffer(out_buffer, sizeof(out_buffer));
    if (!pb_encode(&out_stream, DebugMessage_fields, &msg))
    {
        LOG_ERROR("Error encoding chimera message output");
        return;
    }
    uint16_t response_data_size = (uint16_t)out_stream.bytes_written;

    // Construct respopnse packet.
    // CHIMERA Packet Format:
    // [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]
    uint16_t response_packet_size = 3 + response_data_size;
    uint8_t  response_packet[response_packet_size];
    response_packet[0] = 0x01;
    response_packet[1] = (uint8_t)(response_data_size & 0xff);
    response_packet[2] = (uint8_t)(response_data_size >> 8);
    memcpy(&response_packet[3], out_buffer, response_data_size);

    // Transmit.
    hw_usb_transmit(response_packet, response_packet_size);
}

void io_chimera_v2_main(
    Gpio       *gpio_conf[],
    AdcChannel *adc_conf[],
    uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len),
    uint32_t net_name_gpio,
    uint32_t net_name_adc)
{
    // Store adc and gpio tables.
    id_to_gpio = gpio_conf;
    id_to_adc  = adc_conf;

    // Init usb peripheral.
    hw_usb_init(transmit_handle);

    // If usb is not connected, continue.
    if (!hw_usb_checkConnection())
        return;

    // dump the queue.
    for (;;)
    {
        // CHIMERA Packet Format:
        // [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]
        if (hw_usb_recieve() == 0x00)
            continue;

        // Length-delimination sent with little-endian.
        uint16_t low  = (uint16_t)hw_usb_recieve();
        uint16_t high = (uint16_t)hw_usb_recieve();

        uint16_t length = (uint16_t)(low + (high << 8));

        // Populate content.
        uint8_t content[length];
        for (int i = 0; i < length; i += 1)
            content[i] = hw_usb_recieve();

        io_chimera_v2_handleContent(content, length, net_name_gpio, net_name_adc);
    }
}