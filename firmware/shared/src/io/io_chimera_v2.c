#include "hw_usb.h"
#include <stdlib.h>
#include "io_log.h"
#include <assert.h>
#include "io_log.h"

// Protobuf.
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

// Peripheral interfaces.
#include "hw_adc.h"
#include "hw_gpio.h"

static Gpio       **id_to_gpio;
static AdcChannel **id_to_adc;

// Maximum size for the output rpc content we support (2 bytes).
const uint16_t OUT_BUFFER_SIZE = 0xffff;

// Note: this buffer is allocated at runtime, since on non-Chimera runs we do not want to allocate it.
static pb_byte_t *out_buffer = NULL;

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
    pb_ostream_t out_stream = pb_ostream_from_buffer(out_buffer, OUT_BUFFER_SIZE);
    if (!pb_encode(&out_stream, DebugMessage_fields, &msg))
    {
        LOG_ERROR("Error encoding chimera message output");
        return;
    }
    uint16_t response_data_size = (uint16_t)out_stream.bytes_written;

    // Construct respopnse packet.
    // CHIMERA Packet Format:
    // [ length low byte  | length high byte | content bytes    | ... ]
    uint16_t response_packet_size = 2 + response_data_size;
    uint8_t  response_packet[response_packet_size];
    response_packet[0] = (uint8_t)(response_data_size & 0xff);
    response_packet[1] = (uint8_t)(response_data_size >> 8);
    memcpy(&response_packet[2], out_buffer, response_data_size);

    // Transmit.
    LOG_INFO("Chimera: Sending response packet of size %d", response_packet_size);
    LOG_INFO("Chimera: Response packet:");
    for (int i = 0; i < response_packet_size; i += 1)
        LOG_PRINTF("%02x ", response_packet[i]);
    LOG_PRINTF("\n");

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

    // If usb is not connected, skip Chimera.
    if (!hw_usb_checkConnection())
    {
        LOG_INFO("Chimera: Skipping Chimera - USB not plugged in");
        return;
    }

    // Allocate the output buffer.
    out_buffer = malloc(OUT_BUFFER_SIZE);

    for (uint32_t requests_processed = 1; true; requests_processed += 1)
    {
        LOG_INFO("Chimera: Waiting for message...");

        // CHIMERA Packet Format:
        // [ length low byte  | length high byte | content bytes    | ... ]

        // Get length bytes.
        uint8_t length_bytes[2] = { 0, 0 };
        if (hw_usb_receive(length_bytes, 2) == -1)
            LOG_ERROR("Chimera: Error receiving length bytes");

        // Compute length (little endian).
        uint16_t low    = (uint16_t)length_bytes[0];
        uint16_t high   = (uint16_t)length_bytes[1];
        uint16_t length = (uint16_t)(low + (high << 8));
        LOG_INFO("Chimera: RPC packet received of length %d", length);

        // Receive content.
        uint8_t content[length];
        if (hw_usb_receive(content, length) == -1)
            LOG_ERROR("Chimera: Error receiving message conent.");

        LOG_INFO("Chimera: Received content bytes (without length header):");
        for (int i = 0; i < length; i += 1)
            LOG_PRINTF("%02x ", content[i]);
        LOG_PRINTF("\n");

        io_chimera_v2_handleContent(content, length, net_name_gpio, net_name_adc);

        LOG_INFO("Chimera: Processed %d requests", requests_processed);
    }

    free(out_buffer);
    out_buffer = NULL;
}