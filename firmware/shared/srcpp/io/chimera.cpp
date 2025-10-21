#include "io/chimera.hpp"

#include <cstdint>
#include <cassert>

#include "shared.pb.h"
#include <pb_decode.h>
#include <pb_encode.h>
#include "VC.pb.h"

#include "cmsis_os.h"

#include "hw/adc.hpp"

extern const hw::Gpio *id_to_gpio(uint32_t net_name);
extern const hw::Adc  *id_to_adc(uint32_t net_name);
static const hw::Gpio *io_chimera_parseNetLabelGpio(const GpioNetName *net_name)
{
    switch (net_name->which_name)
    {
        CASERETURN(GpioNetName_vc_net_name_tag, id_to_gpio(net_name->name.vc_net_name));
        CASERETURN(GpioNetName_bms_net_name_tag, id_to_gpio(net_name->name.bms_net_name));
        CASERETURN(GpioNetName_fsm_net_name_tag, id_to_gpio(net_name->name.fsm_net_name));
        CASERETURN(GpioNetName_rsm_net_name_tag, id_to_gpio(net_name->name.rsm_net_name));
        CASERETURN(GpioNetName_crit_net_name_tag, id_to_gpio(net_name->name.crit_net_name));
        default:
            assert(false);
            return NULL;
    }
}
static const hw::Adc *io_chimera_parseNetLabelAdc(const AdcNetName *net_name)
{
    switch (net_name->which_name)
    {
        CASERETURN(AdcNetName_vc_net_name_tag, id_to_adc(net_name->name.vc_net_name));
        CASERETURN(AdcNetName_bms_net_name_tag, id_to_adc(net_name->name.bms_net_name));
        CASERETURN(AdcNetName_fsm_net_name_tag, id_to_adc(net_name->name.fsm_net_name));
        CASERETURN(AdcNetName_rsm_net_name_tag, id_to_adc(net_name->name.rsm_net_name));
        CASERETURN(AdcNetName_crit_net_name_tag, id_to_adc(net_name->name.crit_net_name));
        default:
            assert(false);
            return NULL;
    }
}

#define MAX_DEBUG_BUF_SIZE 100
#define DEBUG_SIZE_MSG_BUF_SIZE 1

namespace io::chimera
{
static const hw::Uart *uart;
static uint32_t        net_name_gpio;
static uint32_t        net_name_adc;
static const hw::Gpio *n_chimera_gpio;

static bool    is_mid_debug_msg = false;
static uint8_t rx_packet_size   = 0;

static uint8_t data[MAX_DEBUG_BUF_SIZE];
static bool    chimera_button_pressed;

void init(const hw::Uart *serial_uart, uint32_t name_gpio, uint32_t name_adc, const hw::Gpio *bootup_gpio)
{
    uart           = serial_uart;
    net_name_gpio  = name_gpio;
    net_name_adc   = name_adc;
    n_chimera_gpio = bootup_gpio;

    // Button is active low
    chimera_button_pressed = !n_chimera_gpio->readPin();

    if (chimera_button_pressed)
    {
        uart->receiveIt(data, DEBUG_SIZE_MSG_BUF_SIZE);
    }
}

void sleepTaskIfEnabled()
{
    if (chimera_button_pressed)
    {
        osDelay(osWaitForever);
    }
}

void msgRxCallback()
{
    if (is_mid_debug_msg)
    {
        DebugMessage msg       = DebugMessage_init_zero;
        pb_istream_t in_stream = pb_istream_from_buffer(data, rx_packet_size);
        assert(pb_decode(&in_stream, DebugMessage_fields, &msg));

        switch (msg.which_payload)
        {
            case DebugMessage_gpio_read_tag:
            {
                // GPIO read message.
                assert(msg.payload.gpio_read.net_name.which_name == net_name_gpio);
                const hw::Gpio *gpio = io_chimera_parseNetLabelGpio(&msg.payload.gpio_read.net_name);
                msg.payload.gpio_read.value =
                    static_cast<GpioValue>((int)gpio->readPin() + 1); // add one for enum scale offset
                break;
            }
            case DebugMessage_gpio_write_tag:
            {
                // GPIO write message.
                assert(msg.payload.gpio_write.net_name.which_name == net_name_gpio);
                const hw::Gpio *gpio = io_chimera_parseNetLabelGpio(&msg.payload.gpio_write.net_name);
                gpio->writePin((int)msg.payload.gpio_write.value - 1); // add one for enum scale offset
                break;
            }
            case DebugMessage_adc_tag:
            {
                // ADC read message.
                assert(msg.payload.adc.net_name.which_name == net_name_adc);
                const hw::Adc *adc_channel = io_chimera_parseNetLabelAdc(&msg.payload.adc.net_name);
                msg.payload.adc.value      = adc_channel->getVoltage();
                break;
            }
            default:
                break;
        }

        // Encode and send reply.
        pb_ostream_t out_stream = pb_ostream_from_buffer(data, sizeof(data));
        assert(pb_encode(&out_stream, DebugMessage_fields, &msg));
        auto tx_packet_size = (uint8_t)out_stream.bytes_written;

        uart->transmitPoll(&tx_packet_size, DEBUG_SIZE_MSG_BUF_SIZE, osWaitForever);
        uart->transmitPoll(data, tx_packet_size, osWaitForever);

        // Wait for next length message (1 byte).
        rx_packet_size   = DEBUG_SIZE_MSG_BUF_SIZE;
        is_mid_debug_msg = false;
    }
    else
    {
        // Received length of the command, wait to receive it.
        rx_packet_size   = data[0];
        is_mid_debug_msg = true;
    }

    // Start receiving data in interrupt mode again so this interrupt will get fired if
    // more data is recieved.
    uart->receiveIt(data, rx_packet_size);
}
} // namespace io::chimera
