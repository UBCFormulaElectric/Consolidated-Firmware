#include <stdlib.h>
#include "main.h"
#include "io_chimera_v2.h"
#include "hw_usb.h"
#include "io_log.h"

// Protobuf.
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

// Maximum size for the output rpc content we support (length specified by 2 bytes, so 2^16 - 1).
// Yes, this is 65kb of RAM - it's a lot, but doable.
#define OUT_BUFFER_SIZE (0xffff)
static pb_byte_t out_buffer[OUT_BUFFER_SIZE];

bool io_chimera_v2_enabled = false;

// Active chimera config.
static io_chimera_v2_Config *chimera_v2_config;

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hw_gpio.h"

// Convert a given GpioNetName to a GPIO pin.
static const Gpio *io_chimera_v2_getGpio(const GpioNetName *net_name)
{
    if (chimera_v2_config->gpio_net_name_tag != net_name->which_name)
    {
        LOG_ERROR(
            "Chimera: Expected GPIO net name with tag %d, got %d", chimera_v2_config->gpio_net_name_tag,
            net_name->which_name);
        return NULL;
    }

    if (net_name->which_name == GpioNetName_f4dev_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.f4dev_net_name];
    if (net_name->which_name == GpioNetName_ssm_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.ssm_net_name];
    if (net_name->which_name == GpioNetName_crit_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.crit_net_name];
    if (net_name->which_name == GpioNetName_dam_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.dam_net_name];
    if (net_name->which_name == GpioNetName_fsm_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.fsm_net_name];
    if (net_name->which_name == I2cNetName_rsm_net_name_tag)
        return chimera_v2_config->id_to_gpio[net_name->name.rsm_net_name];

    LOG_ERROR("Chimera: Received GPIO pin from unsupported board.");
    return NULL;
}
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "hw_adc.h"

// Convert a given AdcNetName to an ADC channel pin.
static const AdcChannel *io_chimera_v2_getAdc(const AdcNetName *net_name)
{
    if (chimera_v2_config->adc_net_name_tag != net_name->which_name)
    {
        LOG_ERROR(
            "Chimera: Expected ADC net name with tag %d, got %d", chimera_v2_config->adc_net_name_tag,
            net_name->which_name);
        return NULL;
    }

    if (net_name->which_name == AdcNetName_f4dev_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.f4dev_net_name];
    if (net_name->which_name == AdcNetName_ssm_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.ssm_net_name];
    if (net_name->which_name == AdcNetName_crit_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.crit_net_name];
    if (net_name->which_name == AdcNetName_dam_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.dam_net_name];
    if (net_name->which_name == AdcNetName_fsm_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.fsm_net_name];
    if (net_name->which_name == AdcNetName_rsm_net_name_tag)
        return chimera_v2_config->id_to_adc[net_name->name.rsm_net_name];

    LOG_ERROR("Chimera: Received ADC channel from unsupported board.");
    return NULL;
}
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "hw_i2c.h"

// Convert a given I2C enum to an I2C device.
static const I2cDevice *io_chimera_v2_getI2c(const I2cNetName *net_name)
{
    if (chimera_v2_config->adc_net_name_tag != net_name->which_name)
    {
        LOG_ERROR(
            "Chimera: Expected I2C net name with tag %d, got %d", chimera_v2_config->i2c_net_name_tag,
            net_name->which_name);
        return NULL;
    }

    if (net_name->which_name == I2cNetName_f4dev_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.f4dev_net_name];
    if (net_name->which_name == I2cNetName_ssm_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.ssm_net_name];
    if (net_name->which_name == I2cNetName_crit_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.crit_net_name];
    if (net_name->which_name == I2cNetName_dam_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.dam_net_name];
    if (net_name->which_name == I2cNetName_fsm_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.fsm_net_name];
    if (net_name->which_name == I2cNetName_rsm_net_name_tag)
        return chimera_v2_config->id_to_i2c[net_name->name.rsm_net_name];

    LOG_ERROR("Chimera: Received I2C device from unsupported board.");
    return 0;
}
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "hw_spi.h"

// Convert a given SPI enum to a SPI device.
static const SpiDevice *io_chimera_v2_getSpi(const SpiNetName *net_name)
{
    if (chimera_v2_config->spi_net_name_tag != net_name->which_name)
    {
        LOG_ERROR(
            "Expected SPI net name with tag %d, got %d", chimera_v2_config->spi_net_name_tag, net_name->which_name);
        return NULL;
    }

    if (net_name->which_name == SpiNetName_f4dev_net_name_tag)
        return chimera_v2_config->id_to_spi[net_name->name.f4dev_net_name];
    if (net_name->which_name == SpiNetName_ssm_net_name_tag)
        return chimera_v2_config->id_to_spi[net_name->name.ssm_net_name];
    if (net_name->which_name == SpiNetName_crit_net_name_tag)
        return chimera_v2_config->id_to_spi[net_name->name.crit_net_name];
    if (net_name->which_name == SpiNetName_fsm_net_name_tag)
        return chimera_v2_config->id_to_spi[net_name->name.fsm_net_name];
    if (net_name->which_name == SpiNetName_rsm_net_name_tag)
        return chimera_v2_config->id_to_spi[net_name->name.rsm_net_name];

    LOG_ERROR("Received SPI device from unsupported board.");
    return NULL;
}
#endif

// Handle an rpc message.
void io_chimera_v2_handleContent(uint8_t *content, uint16_t length)
{
    // Setup request.
    ChimeraV2Request request        = ChimeraV2Request_init_zero;
    pb_istream_t     content_stream = pb_istream_from_buffer(content, length);
    if (!pb_decode(&content_stream, ChimeraV2Request_fields, &request))
    {
        LOG_ERROR("Chimera: Error decoding chimera message stream.");
        return;
    }

    // Setup response.
    ChimeraV2Response response = ChimeraV2Response_init_zero;

    /* GPIO commands. */
#ifdef HAL_GPIO_MODULE_ENABLED
    if (request.which_payload == ChimeraV2Request_gpio_read_tag)
    {
        // Extract payload
        GpioReadRequest *payload = &request.payload.gpio_read;

        // GPIO read.
        const Gpio *gpio  = io_chimera_v2_getGpio(&payload->net_name);
        bool        value = hw_gpio_readPin(gpio);

        // Format response.
        response.which_payload           = ChimeraV2Response_gpio_read_tag;
        response.payload.gpio_read.value = value;
    }
    else if (request.which_payload == ChimeraV2Request_gpio_write_tag)
    {
        // Extract payload
        GpioWriteRequest *payload = &request.payload.gpio_write;

        // GPIO write.
        const Gpio *gpio = io_chimera_v2_getGpio(&payload->net_name);
        hw_gpio_writePin(gpio, request.payload.gpio_write.value);

        // Format response.
        response.which_payload              = ChimeraV2Response_gpio_write_tag;
        response.payload.gpio_write.success = true;
    }
#endif

    /* ADC commands. */
#ifdef HAL_ADC_MODULE_ENABLED
    else if (request.which_payload == ChimeraV2Request_adc_read_tag)
    {
        // Extract payload
        AdcReadRequest *payload = &request.payload.adc_read;

        // ADC read.
        const AdcChannel *adc_channel = io_chimera_v2_getAdc(&payload->net_name);
        float             value       = hw_adc_getVoltage(adc_channel);

        // Format response.
        response.which_payload          = ChimeraV2Response_adc_read_tag;
        response.payload.adc_read.value = value;
    }
#endif

    /* I2C commands. */
#ifdef HAL_I2C_MODULE_ENABLED
    else if (request.which_payload == ChimeraV2Request_i2c_ready_tag)
    {
        // Extract payload
        I2cReadyRequest *payload = &request.payload.i2c_ready;

        // I2C ready check.
        const I2cDevice *device = io_chimera_v2_getI2c(&payload->net_name);
        bool             ready  = hw_i2c_isTargetReady(device);

        // Format response.
        response.which_payload           = ChimeraV2Response_i2c_ready_tag;
        response.payload.i2c_ready.ready = ready;
    }
    else if (request.which_payload == ChimeraV2Request_i2c_transmit_tag)
    {
        // Extract payload
        I2cTransmitRequest *payload = &request.payload.i2c_transmit;
        const I2cDevice    *device  = io_chimera_v2_getI2c(&payload->net_name);

        bool success = hw_i2c_transmit(device, payload->data.bytes, payload->data.size);

        // Format response.
        response.which_payload                = ChimeraV2Response_i2c_transmit_tag;
        response.payload.i2c_transmit.success = success;
    }
    else if (request.which_payload == ChimeraV2Request_i2c_memory_write_tag)
    {
        // Extract payload
        I2cMemoryWriteRequest *payload = &request.payload.i2c_memory_write;
        const I2cDevice       *device  = io_chimera_v2_getI2c(&payload->net_name);

        bool success =
            hw_i2c_memoryWrite(device, (uint16_t)payload->memory_address, payload->data.bytes, payload->data.size);

        // Format response.
        response.which_payload                    = ChimeraV2Response_i2c_memory_write_tag;
        response.payload.i2c_memory_write.success = success;
    }
    else if (request.which_payload == ChimeraV2Request_i2c_receive_tag)
    {
        // Extract payload
        I2cReceiveRequest *payload = &request.payload.i2c_receive;

        const I2cDevice *device = io_chimera_v2_getI2c(&payload->net_name);
        uint8_t          data[payload->length];
        bool             success = hw_i2c_receive(device, data, (uint16_t)payload->length);
        if (!success)
            LOG_ERROR("Chimera: Failed to receive on I2C");

        // Format response.
        response.which_payload = ChimeraV2Response_i2c_receive_tag;

        response.payload.i2c_receive.data.size = (pb_size_t)payload->length;
        for (size_t i = 0; i < payload->length; i += 1)
        {
            response.payload.i2c_receive.data.bytes[i] = data[i];
        }
    }
    else if (request.which_payload == ChimeraV2Request_i2c_memory_read_tag)
    {
        // Extract payload
        I2cMemoryReadRequest *payload = &request.payload.i2c_memory_read;
        const I2cDevice      *device  = io_chimera_v2_getI2c(&payload->net_name);

        uint8_t data[payload->length];
        bool    success = hw_i2c_memoryRead(device, (uint16_t)payload->memory_address, data, (uint16_t)payload->length);
        if (!success)
            LOG_ERROR("Chimera: Failed to receive on I2C");

        // Format response.
        response.which_payload = ChimeraV2Response_i2c_memory_read_tag;

        response.payload.i2c_memory_read.data.size = (pb_size_t)payload->length;
        for (size_t i = 0; i < payload->length; i += 1)
        {
            response.payload.i2c_memory_read.data.bytes[i] = data[i];
        }
    }
#endif

    /* SPI commands. */
#ifdef HAL_SPI_MODULE_ENABLED
    else if (request.which_payload == ChimeraV2Request_spi_receive_tag)
    {
        // Extract payload.
        SpiReceiveRequest *payload = &request.payload.spi_receive;
        const SpiDevice   *device  = io_chimera_v2_getSpi(&payload->net_name);

        // Read data.
        uint8_t data[payload->length];
        bool    success = hw_spi_receive(device, data, (uint16_t)payload->length);
        if (!success)
            LOG_ERROR("Chimera: Failed to receive on SPI");

        // Format response.
        response.which_payload                 = ChimeraV2Response_spi_receive_tag;
        response.payload.spi_receive.data.size = (pb_size_t)payload->length;
        for (size_t i = 0; i < payload->length; i += 1)
        {
            response.payload.spi_receive.data.bytes[i] = data[i];
        }
    }
    else if (request.which_payload == ChimeraV2Request_spi_transmit_tag)
    {
        // Extract payload.
        SpiTransmitRequest *payload = &request.payload.spi_transmit;
        const SpiDevice    *device  = io_chimera_v2_getSpi(&payload->net_name);

        // Transmit data.
        bool success = hw_spi_transmit(device, payload->data.bytes, (uint16_t)payload->data.size);

        // Format response.
        response.which_payload                = ChimeraV2Response_spi_transmit_tag;
        response.payload.spi_transmit.success = success;
    }
    else if (request.which_payload == ChimeraV2Request_spi_transaction_tag)
    {
        // Extract payload.
        SpiTransactionRequest *payload = &request.payload.spi_transaction;
        const SpiDevice       *device  = io_chimera_v2_getSpi(&payload->net_name);

        // Transact data.
        uint8_t rx_data[payload->rx_length];
        bool    success = hw_spi_transmitThenReceive(
            device, payload->tx_data.bytes, payload->tx_data.size, rx_data, (uint16_t)payload->rx_length);
        if (!success)
            LOG_ERROR("Chimera: Failed to Transact on SPI");

        // Format response.
        response.which_payload                        = ChimeraV2Response_spi_transaction_tag;
        response.payload.spi_transaction.rx_data.size = (pb_size_t)payload->rx_length;
        for (size_t i = 0; i < payload->rx_length; i += 1)
        {
            response.payload.spi_transaction.rx_data.bytes[i] = rx_data[i];
        }
    }
#endif

    else
    {
        LOG_WARN("Chimera: Unsupported request with tag %d received.", request.which_payload);
    }

    // Encode response to bytes.
    pb_ostream_t out_stream = pb_ostream_from_buffer(out_buffer, OUT_BUFFER_SIZE);
    if (!pb_encode(&out_stream, ChimeraV2Response_fields, &response))
    {
        LOG_ERROR("Chimera: Error encoding chimera message output.");
        return;
    }
    uint16_t response_data_size = (uint16_t)out_stream.bytes_written;

    // Construct response packet.
    // CHIMERA Packet Format:
    // [ length low byte  | length high byte | content bytes    | ... ]
    uint16_t response_packet_size = 2 + response_data_size;
    uint8_t  response_packet[response_packet_size];
    response_packet[0] = (uint8_t)(response_data_size & 0xff);
    response_packet[1] = (uint8_t)(response_data_size >> 8);
    memcpy(&response_packet[2], out_buffer, response_data_size);

    // Transmit.
    LOG_INFO("Chimera: Sending response packet of size %d.", response_packet_size);
    LOG_INFO("Chimera: Response packet:");
    for (int i = 0; i < response_packet_size; i += 1)
        LOG_PRINTF("%02x ", response_packet[i]);
    LOG_PRINTF("\n");

    if (!hw_usb_transmit(response_packet, response_packet_size))
        LOG_ERROR("Chimera: Error transmitting response packet.");
}

void io_chimera_v2_mainOrContinue(io_chimera_v2_Config *config)
{
    io_chimera_v2_enabled = true;

    // Keep track of config.
    chimera_v2_config = config;

    // Main loop.
    for (uint32_t requests_processed = 1; true; requests_processed += 1)
    {
        // If usb is not connected, skip Chimera.
        if (!hw_usb_checkConnection())
        {
            LOG_INFO("Chimera: Skipping Chimera - USB not plugged in.");
            break;
        }

        LOG_INFO("Chimera: Waiting for message...");
        // CHIMERA Packet Format:
        // [ length low byte  | length high byte | content bytes    | ... ]

        // Get length bytes.
        uint8_t length_bytes[2] = { 0, 0 };
        if (!hw_usb_receive(length_bytes, 2))
            LOG_ERROR("Chimera: Error receiving length bytes.");

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

        // Parse content and return response.
        io_chimera_v2_handleContent(content, length);

        LOG_INFO("Chimera: Processed %d requests", requests_processed);
    }

    io_chimera_v2_enabled = false;
}