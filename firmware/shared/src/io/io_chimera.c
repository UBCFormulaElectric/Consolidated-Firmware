#include "io_chimera.h"
#include <stdint.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <assert.h>
#include "cmsis_os.h"

#include "hw_adc.h"

#include "shared.pb.h"
#include "VC.pb.h"

#define MAX_DEBUG_BUF_SIZE 100
#define DEBUG_SIZE_MSG_BUF_SIZE 1

extern const Gpio *const id_to_gpio[];
extern const AdcChannel  id_to_adc[];

static const UART *uart = NULL;
static bool        is_mid_debug_msg;
static uint8_t     data[MAX_DEBUG_BUF_SIZE];
static uint8_t     rx_packet_size;
static uint32_t    net_name_gpio;
static uint32_t    net_name_adc;
static bool        chimera_button_pressed;

static const Gpio *n_chimera_gpio = NULL;

static const Gpio *io_chimera_parseNetLabelGpio(const GpioNetName *net_name)
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

static AdcChannel io_chimera_parseNetLabelAdc(const AdcNetName *net_name)
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
            return 0U;
        }
    }
}

void io_chimera_init(const UART *serial_uart, uint32_t name_gpio, uint32_t name_adc, const Gpio *bootup_gpio)
{
    uart             = serial_uart;
    is_mid_debug_msg = false;
    rx_packet_size   = 0;
    net_name_gpio    = name_gpio;
    net_name_adc     = name_adc;

    n_chimera_gpio = bootup_gpio;

    // Button is active low
    chimera_button_pressed = hw_gpio_readPin(n_chimera_gpio) ? false : true;

    if (chimera_button_pressed)
    {
        hw_uart_receiveIt(uart, data, DEBUG_SIZE_MSG_BUF_SIZE);
    }
}

void io_chimera_sleepTaskIfEnabled(void)
{
    if (chimera_button_pressed)
    {
        osDelay(osWaitForever);
    }
}

void io_chimera_msgRxCallback(void)
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
                const Gpio *gpio            = io_chimera_parseNetLabelGpio(&msg.payload.gpio_read.net_name);
                msg.payload.gpio_read.value = hw_gpio_readPin(gpio) + 1; // add one for enum scale offset
                break;
            }
            case DebugMessage_gpio_write_tag:
            {
                // GPIO write message.
                assert(msg.payload.gpio_write.net_name.which_name == net_name_gpio);
                const Gpio *gpio = io_chimera_parseNetLabelGpio(&msg.payload.gpio_write.net_name);
                hw_gpio_writePin(gpio, msg.payload.gpio_write.value - 1); // add one for enum scale offset
                break;
            }
            case DebugMessage_adc_tag:
            {
                // ADC read message.
                assert(msg.payload.adc.net_name.which_name == net_name_adc);
                const AdcChannel adc_channel = io_chimera_parseNetLabelAdc(&msg.payload.adc.net_name);
                msg.payload.adc.value        = hw_adc_getVoltage(adc_channel);
                break;
            }
            default:
                break;
        }

        // Encode and send reply.
        pb_ostream_t out_stream = pb_ostream_from_buffer(data, sizeof(data));
        assert(pb_encode(&out_stream, DebugMessage_fields, &msg));
        uint8_t tx_packet_size = (uint8_t)out_stream.bytes_written;

        hw_uart_transmitPoll(uart, &tx_packet_size, DEBUG_SIZE_MSG_BUF_SIZE, osWaitForever);
        hw_uart_transmitPoll(uart, data, tx_packet_size, osWaitForever);

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
    hw_uart_receiveIt(uart, data, rx_packet_size);
}
