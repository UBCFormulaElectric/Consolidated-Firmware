#include "hw_usb.h"
#include <stdlib.h>
#include "io_log.h"

// Protobuf.
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

// Peripheral interfaces.
#include "hw_adcs.h"
#include "hw_gpios.h"

// Get ID to GPIO/ADC tables.
extern const Gpio *const       id_to_gpio[];
extern const AdcChannel *const id_to_adc[];

static const Gpio *io_chimera_v2_parseNetLabelGpio(const GpioNetName *net_name)
{
    switch (net_name->which_name)
    {
        case GpioNetName_vc_net_name_tag:
        {
            return id_to_gpio[net_name->name.vc_net_name];
        }
        case GpioNetName_bms_net_name_tag:
        {
            return id_to_gpio[net_name->name.bms_net_name];
        }
        case GpioNetName_fsm_net_name_tag:
        {
            return id_to_gpio[net_name->name.fsm_net_name];
        }
        case GpioNetName_rsm_net_name_tag:
        {
            return id_to_gpio[net_name->name.rsm_net_name];
        }
        case GpioNetName_crit_net_name_tag:
        {
            return id_to_gpio[net_name->name.crit_net_name];
        }
        default:
        {
            assert(false);
            return 0U;
        }
    }
}

static const AdcChannel *io_chimera_v2_parseNetLabelAdc(const AdcNetName *net_name)
{
    switch (net_name->which_name)
    {
        case AdcNetName_vc_net_name_tag:
        {
            return id_to_adc[net_name->name.vc_net_name];
        }
        case AdcNetName_bms_net_name_tag:
        {
            return id_to_adc[net_name->name.bms_net_name];
        }
        case AdcNetName_fsm_net_name_tag:
        {
            return id_to_adc[net_name->name.fsm_net_name];
        }
        case AdcNetName_rsm_net_name_tag:
        {
            return id_to_adc[net_name->name.rsm_net_name];
        }
        case AdcNetName_crit_net_name_tag:
        {
            return id_to_adc[net_name->name.crit_net_name];
        }
        default:
        {
            assert(false);
            return NULL;
        }
    }
}

void io_chimera_v2_main(
    uint8_t (*transmit_handle)(uint8_t *Buf, uint16_t Len),
    uint32_t net_name_gpio,
    uint32_t net_name_adc)
{
    // Init usb peripheral.
    hw_usb_init(transmit_handle);

    // If usb is not connected, continue.
    if (!hw_usb_checkConnection())
        return;

    // dump the queue.
    for (int i = 0; true; i += 1)
    {
        // CHIMERA Packet Format:
        // [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]
        if (hw_usb_recieve() == 0x0)
            continue;

        // Length-delimination sent with little-endian.
        uint8_t low_byte  = hw_usb_recieve();
        uint8_t high_byte = hw_usb_recieve();

        // Extract length.
        uint16_t length = low_byte + high_byte << 8;

        // Populate buffer.
        uint8_t buf[length];
        for (int i = 0; i < length; i += 1)
            buf[i] = hw_usb_recieve();

        io_chimera_handleBuf(buf, length);
    }
};

void io_chimera_v2_handleBuf(uint8_t *buf, uint16_t length)
{
    // Receive message.
    DebugMessage msg    = DebugMessage_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buf, length);
    bool         status = pb_decode(&stream, DebugMessage_fields, &msg);
    if (!status)
    {
        LOG_ERROR("Error decoding chimera message stream");
        return;
    }

    switch (msg.which_payload)
    {
        case DebugMessage_gpio_read_tag:
        {
            // GPIO read message.
            assert(msg.payload.gpio_read.net_name.which_name == net_name_gpio);
            const Gpio *gpio            = io_chimera_v2_parseNetLabelGpio(&msg.payload.gpio_read.net_name);
            msg.payload.gpio_read.value = hw_gpio_readPin(gpio) + 1; // add one for enum scale offset
            break;
        }
        case DebugMessage_gpio_write_tag:
        {
            // GPIO write message.
            assert(msg.payload.gpio_write.net_name.which_name == net_name_gpio);
            const Gpio *gpio = io_chimera_v2_parseNetLabelGpio(&msg.payload.gpio_write.net_name);
            hw_gpio_writePin(gpio, msg.payload.gpio_write.value - 1); // add one for enum scale offset
            break;
        }
        case DebugMessage_adc_tag:
        {
            // ADC read message.
            assert(msg.payload.adc.net_name.which_name == net_name_adc);
            const AdcChannel *adc_channel = io_chimera_v2_parseNetLabelAdc(&msg.payload.adc.net_name);
            msg.payload.adc.value         = hw_adc_getVoltage(adc_channel);
            break;
        }
        default:
            break;
    }

    // Encode and send reply.
    pb_ostream_t out_stream = pb_ostream_from_buffer(data, sizeof(data));
    assert(pb_encode(&out_stream, DebugMessage_fields, &msg));
    uint16_t response_data_size = (uint16_t)out_stream.bytes_written;

    // Construct respopnse packet.
    // CHIMERA Packet Format:
    // [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]
    uint16_t response_packet_size = 3 + response_data_size;
    uint8_t  response_packet[response_packet_size];
    response_packet[0] = 0x01;
    response_packet[1] = response_data_size & 0xff;
    response_packet[2] = response_data_size >> 8;
    memcpy(&response_packet[3], msg, response_data_size);

    hw_usb_transmit(response_packet, response_packet_size);
}