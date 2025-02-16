#include <stdlib.h>
#include <assert.h>
#include "hw_usb.h"
#include "io_log.h"

// Protobuf.
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

// Peripheral interfaces.
#include "hw_adc.h"
#include "hw_gpio.h"

static const Gpio       **id_to_gpio;
static const AdcChannel **id_to_adc;

pb_size_t gpio_net_name_tag = 0;
pb_size_t adc_net_name_tag  = 0;

// Maximum size for the output rpc content we support (length specified by 2 bytes, so 2^16 - 1).
const uint16_t OUT_BUFFER_SIZE = 0xffff;

// Note: this buffer is allocated at runtime, since on non-Chimera runs we do not want to allocate it.
static pb_byte_t *out_buffer = NULL;

// Convert a given GpioNetName to a GPIO pin.
static const Gpio *io_chimera_v2_getGpio(const GpioNetName *net_name)
{
    if (gpio_net_name_tag != net_name->which_name)
        LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, net_name->which_name);

    if (net_name->which_name == GpioNetName_f4dev_net_name_tag)
        return id_to_gpio[net_name->name.f4dev_net_name];

    return NULL;
}

// Convert a given AdcNetName to an ADC channel pin.
static const AdcChannel *io_chimera_v2_getAdc(const AdcNetName *net_name)
{
    if (adc_net_name_tag != net_name->which_name)
        LOG_ERROR("Chimera: Expected ADC net name with tag %d, got %d", adc_net_name_tag, net_name->which_name);

    if (net_name->which_name == AdcNetName_f4dev_net_name_tag)
        return id_to_adc[net_name->name.f4dev_net_name];

    return NULL;
}

// Handle an rpc message.
void io_chimera_v2_handleContent(uint8_t *content, uint16_t length)
{
    // Setup request.
    ChimeraV2Request request        = ChimeraV2Request_init_zero;
    pb_istream_t     content_stream = pb_istream_from_buffer(content, length);
    if (!pb_decode(&content_stream, ChimeraV2Request_fields, &request))
    {
        LOG_ERROR("Chimera: Error decoding chimera message stream");
        return;
    }

    // Setup response.
    ChimeraV2Response response = ChimeraV2Response_init_zero;

    if (request.which_payload == ChimeraV2Request_gpio_read_tag)
    {
        // GPIO read.
        const Gpio *gpio = io_chimera_v2_getGpio(&request.payload.gpio_read.net_name);
        bool        data = hw_gpio_readPin(gpio);

        // Format response.
        response.which_payload              = ChimeraV2Response_gpio_read_tag;
        response.payload.gpio_read.value    = data;
        response.payload.gpio_read.net_name = request.payload.gpio_read.net_name;
    }
    else if (request.which_payload == ChimeraV2Request_gpio_write_tag)
    {
        // GPIO write.
        const Gpio *gpio = io_chimera_v2_getGpio(&request.payload.gpio_write.net_name);
        hw_gpio_writePin(gpio, request.payload.gpio_write.value);

        // Format response.
        response.which_payload                           = ChimeraV2Response_gpio_write_tag;
        response.payload.gpio_write.request_confirmation = request.payload.gpio_write;
    }
    else if (request.which_payload == ChimeraV2Request_adc_read_tag)
    {
        // ADC read.
        const AdcChannel *adc_channel = io_chimera_v2_getAdc(&request.payload.adc_read.net_name);
        float             data        = hw_adc_getVoltage(adc_channel);

        // Format response.
        response.which_payload             = ChimeraV2Response_adc_read_tag;
        response.payload.adc_read.value    = data;
        response.payload.adc_read.net_name = request.payload.adc_read.net_name;
    }

    // Encode response to bytes.
    pb_ostream_t out_stream = pb_ostream_from_buffer(out_buffer, OUT_BUFFER_SIZE);
    if (!pb_encode(&out_stream, ChimeraV2Response_fields, &response))
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

    if (!hw_usb_transmit(response_packet, response_packet_size))
        LOG_ERROR("Chimera: Error transmitting response packet.");
}

void io_chimera_v2_main(const Gpio *gpio_conf[], const AdcChannel *adc_conf[], pb_size_t gpio_tag, pb_size_t adc_tag)
{
    // Set tags.
    gpio_net_name_tag = gpio_tag;
    adc_net_name_tag  = adc_tag;

    // Store adc and gpio tables.
    id_to_gpio = gpio_conf;
    id_to_adc  = adc_conf;

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
        if (!hw_usb_receive(length_bytes, 2))
            LOG_ERROR("Chimera: Error receiving length bytes");

        // Compute length (little endian).
        uint16_t low    = (uint16_t)length_bytes[0];
        uint16_t high   = (uint16_t)length_bytes[1];
        uint16_t length = (uint16_t)(low + (high << 8));
        LOG_INFO("Chimera: RPC packet received of length %d", length);

        // Receive content.
        uint8_t content[length];
        if (!hw_usb_receive(content, length))
            LOG_ERROR("Chimera: Error receiving message conent.");

        LOG_INFO("Chimera: Received content bytes (without length header):");
        for (int i = 0; i < length; i += 1)
            LOG_PRINTF("%02x ", content[i]);
        LOG_PRINTF("\n");

        io_chimera_v2_handleContent(content, length);

        LOG_INFO("Chimera: Processed %d requests", requests_processed);
    }

    free(out_buffer);
    out_buffer = NULL;
}