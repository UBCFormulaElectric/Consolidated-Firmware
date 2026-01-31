#include "chimera_v2.hpp"
#include "io_log.hpp"
#include "hw_usb.hpp"
#include "io_queue.hpp"

// Protobuf.
#include <cmsis_os2.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "shared.pb.h"

#include <array>

// Maximum size for the output rpc content we support (length specified by 2 bytes, so 2^16 - 1).
// Yes, this is 65kb of RAM - it's a lot, but doable.
#define OUT_BUFFER_SIZE (0xfff)
static pb_byte_t out_buffer[OUT_BUFFER_SIZE];

constexpr size_t MAX_PAYLOAD_SIZE = 64;

static io::queue<uint8_t, 100> q{ "USBQueue", nullptr, nullptr };

void hw::usb::receive(const std::span<uint8_t> dest)
{
    for (const uint8_t &x : dest)
    {
        assert(q.push(x).has_value());
    }
}

namespace chimera_v2
{
bool enabled = false;

/**
 * @brief Evaluate a Chimera V2 request.
 * @param c Collection of protobuf enum to peripheral tables and net name tags.
 * @param request Request from the Chimera V2 python client.
 * @param response Pointer to the response struct to write the result to.
 * @return True if success, otherwise false.
 */
static bool evaluateRequest(const config &c, const ChimeraV2Request &request, ChimeraV2Response &response)
{
    // Empty provided response pointer.
    constexpr ChimeraV2Response init = ChimeraV2Response_init_zero;
    response                         = init;

    /* GPIO commands. */
#ifdef HAL_GPIO_MODULE_ENABLED
    if (request.which_payload == ChimeraV2Request_gpio_read_tag)
    {
        // Extract payload
        const GpioReadRequest *payload = &request.payload.gpio_read;

        // GPIO read.
        const auto gpio = c.id_to_gpio(&payload->net_name);
        if (not gpio.has_value())
        {
            LOG_ERROR("Chimera: Error fetching GPIO peripheral.");
            return false;
        }
        // Format response.
        response.payload.gpio_read.value = gpio.value().get().readPin();
        response.which_payload           = ChimeraV2Response_gpio_read_tag;
    }
    else if (request.which_payload == ChimeraV2Request_gpio_write_tag)
    {
        // Extract payload
        const GpioWriteRequest *payload = &request.payload.gpio_write;

        // GPIO write.
        const auto gpio = c.id_to_gpio(&payload->net_name);
        if (not gpio.has_value())
        {
            LOG_ERROR("Chimera: Error fetching GPIO peripheral.");
            return false;
        }
        gpio.value().get().writePin(request.payload.gpio_write.value);

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
        const AdcReadRequest *payload = &request.payload.adc_read;

        // ADC read.
        // const AdcChannel *adc_channel = hw_chimera_v2_getAdc(config, &payload->net_name);
        const auto adc_channel = c.id_to_adc(&payload->net_name);
        if (not adc_channel.has_value())
        {
            LOG_ERROR("Chimera: Error fetching ADC peripheral.");
            return false;
        }
        // Format response.
        response.payload.adc_read.value = adc_channel.value().get().getVoltage();
        response.which_payload          = ChimeraV2Response_adc_read_tag;
    }
#endif

    /* I2C commands. */
#ifdef HAL_I2C_MODULE_ENABLED
    else if (request.which_payload == ChimeraV2Request_i2c_ready_tag)
    {
        // Extract payload
        const I2cReadyRequest *payload = &request.payload.i2c_ready;

        // I2C ready check.
        // const I2cDevice *device = hw_chimera_v2_getI2c(config, &payload->net_name);
        const auto device = c.id_to_i2c(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching I2C peripheral.");
            return false;
        }

        // Format response.
        response.payload.i2c_ready.ready = device.value().get().isTargetReady().has_value();
        response.which_payload           = ChimeraV2Response_i2c_ready_tag;
    }
    else if (request.which_payload == ChimeraV2Request_i2c_transmit_tag)
    {
        // Extract payload
        const I2cTransmitRequest *payload = &request.payload.i2c_transmit;
        const auto                device  = c.id_to_i2c(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching I2C peripheral.");
            return false;
        }

        // const bool success = hw_i2c_transmit(device, payload->data.bytes, payload->data.size).has_value();
        const bool success = device.value().get().transmit({ payload->data.bytes, payload->data.size }).has_value();

        // Format response.
        response.which_payload                = ChimeraV2Response_i2c_transmit_tag;
        response.payload.i2c_transmit.success = success;
    }
    else if (request.which_payload == ChimeraV2Request_i2c_memory_write_tag)
    {
        // Extract payload
        const I2cMemoryWriteRequest *payload = &request.payload.i2c_memory_write;
        const auto                   device  = c.id_to_i2c(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching I2C peripheral.");
            return false;
        }

        // Format response.
        response.which_payload = ChimeraV2Response_i2c_memory_write_tag;
        response.payload.i2c_memory_write.success =
            device.value()
                .get()
                .memoryWrite(
                    static_cast<uint16_t>(payload->memory_address), { payload->data.bytes, payload->data.size })
                .has_value();
    }
    else if (request.which_payload == ChimeraV2Request_i2c_receive_tag)
    {
        // Extract payload
        const I2cReceiveRequest *payload = &request.payload.i2c_receive;

        const auto device = c.id_to_i2c(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching I2C peripheral.");
            return false;
        }

        uint8_t data[MAX_PAYLOAD_SIZE];
        if (not device.value().get().receive({ data, static_cast<uint16_t>(payload->length) }).has_value())
        {
            LOG_ERROR("Chimera: Failed to receive on I2C");
            return false;
        }

        // Format response.
        response.which_payload = ChimeraV2Response_i2c_receive_tag;

        response.payload.i2c_receive.data.size = static_cast<pb_size_t>(payload->length);
        for (size_t i = 0; i < payload->length; i += 1)
        {
            response.payload.i2c_receive.data.bytes[i] = data[i];
        }
    }
    else if (request.which_payload == ChimeraV2Request_i2c_memory_read_tag)
    {
        // Extract payload
        const I2cMemoryReadRequest *payload = &request.payload.i2c_memory_read;
        const auto                  device  = c.id_to_i2c(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching I2C peripheral.");
            return false;
        }

        uint8_t data[MAX_PAYLOAD_SIZE];
        if (not device.value()
                    .get()
                    .memoryRead(
                        static_cast<uint16_t>(payload->memory_address),
                        { data, static_cast<uint16_t>(payload->length) })
                    .has_value())
            LOG_ERROR("Chimera: Failed to receive on I2C");

        // Format response.
        response.which_payload = ChimeraV2Response_i2c_memory_read_tag;

        response.payload.i2c_memory_read.data.size = static_cast<pb_size_t>(payload->length);
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
        const SpiReceiveRequest *payload = &request.payload.spi_receive;
        const auto               device  = c.id_to_spi(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching SPI peripheral.");
            return false;
        }

        // Read data.
        uint8_t data[MAX_PAYLOAD_SIZE];
        if (const bool success =
                device.value().get().receive({ data, static_cast<uint16_t>(payload->length) }).has_value();
            !success)
            LOG_ERROR("Chimera: Failed to receive on SPI");

        response.which_payload                 = ChimeraV2Response_spi_receive_tag;
        response.payload.spi_receive.data.size = static_cast<pb_size_t>(payload->length);
        for (size_t i = 0; i < payload->length; i += 1)
        {
            response.payload.spi_receive.data.bytes[i] = data[i];
        }
    }
    else if (request.which_payload == ChimeraV2Request_spi_transmit_tag)
    {
        // Extract payload.
        const SpiTransmitRequest *payload = &request.payload.spi_transmit;
        const auto                device  = c.id_to_spi(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching SPI peripheral.");
            return false;
        }

        response.which_payload = ChimeraV2Response_spi_transmit_tag;
        response.payload.spi_transmit.success =
            device.value()
                .get()
                .transmit({ payload->data.bytes, static_cast<uint16_t>(payload->data.size) })
                .has_value();
    }
    else if (request.which_payload == ChimeraV2Request_spi_transaction_tag)
    {
        // Extract payload.
        const SpiTransactionRequest *payload = &request.payload.spi_transaction;
        const auto                   device  = c.id_to_spi(&payload->net_name);
        if (not device.has_value())
        {
            LOG_ERROR("Chimera: Error fetching SPI peripheral.");
            return false;
        }

        // Transact data.
        uint8_t rx_data[MAX_PAYLOAD_SIZE];
        if (not device.value()
                    .get()
                    .transmitThenReceive(
                        { payload->tx_data.bytes, payload->tx_data.size },
                        { rx_data, static_cast<uint16_t>(payload->rx_length) })
                    .has_value())
        {
            LOG_ERROR("Chimera: Failed to Transact on SPI");
            return false;
        }

        // Format response.
        response.which_payload                        = ChimeraV2Response_spi_transaction_tag;
        response.payload.spi_transaction.rx_data.size = static_cast<pb_size_t>(payload->rx_length);
        for (size_t i = 0; i < payload->rx_length; i += 1)
        {
            response.payload.spi_transaction.rx_data.bytes[i] = rx_data[i];
        }
    }
#endif
    else
    {
        LOG_WARN("Chimera: Unsupported request with tag %d received.", request.which_payload);
        return false;
    }

    return true;
}

/**
 * @brief Handle the content of a Chimera V2 message (parse, evaluate, respond).
 * @param c Collection of protobuf enum to peripheral tables and net name tags.
 * @param content Content buffer to handle.
 * @return True if success, otherwise false.
 */
static bool handleContent(const config &c, std::span<uint8_t> content)
{
    // Keep track if an error occured.
    // We do this instead of immediate returns,
    // since we must always return a response to the Chimera V2 client
    bool error_occurred = false;

    // Setup response.
    ChimeraV2Response response = ChimeraV2Response_init_zero;

    // Parse request.
    ChimeraV2Request request        = ChimeraV2Request_init_zero;
    pb_istream_t     content_stream = pb_istream_from_buffer(content.data(), content.size());
    if (!pb_decode(&content_stream, ChimeraV2Request_fields, &request))
    {
        LOG_ERROR("Chimera: Error decoding chimera message stream.");
        error_occurred = true;
    }

    // Evaluate request.
    if (!evaluateRequest(c, request, response))
    {
        LOG_ERROR("Chimera: Error evaluating request.");
        error_occurred = true;
    };

    // Encode response to bytes.
    pb_ostream_t out_stream = pb_ostream_from_buffer(out_buffer, OUT_BUFFER_SIZE);
    if (!pb_encode(&out_stream, ChimeraV2Response_fields, &response))
    {
        LOG_ERROR("Chimera: Error encoding chimera message output.");
        error_occurred = true;
    }
    auto response_data_size = static_cast<uint16_t>(out_stream.bytes_written);

    // Construct response packet.
    // CHIMERA Packet Format:
    // [ length low byte  | length high byte | content bytes    | ... ]
    uint16_t response_packet_size = 2 + response_data_size;
    uint8_t  response_packet[64]; // TODO fix
    response_packet[0] = static_cast<uint8_t>(response_data_size & 0xff);
    response_packet[1] = static_cast<uint8_t>(response_data_size >> 8);
    memcpy(&response_packet[2], out_buffer, response_data_size);

    // Print response packet for sanity check.
    LOG_INFO("Chimera: Sending response packet of size %d.", response_packet_size);
    LOG_INFO("Chimera: Response packet:");
    for (int i = 0; i < response_packet_size; i += 1)
        _LOG_PRINTF("%02x ", response_packet[i]);
    _LOG_PRINTF("\n");

    // Transmit.
    if (const auto e = hw::usb::transmit({ response_packet, response_packet_size }); not e.has_value())
    {
        LOG_ERROR("Chimera: Error (%d) transmitting response packet.", e.error());
        error_occurred = true;
    }

    return !error_occurred;
}

/**
 * @brief Run a single chimera tick.
 * @param config Collection of protobuf enum to peripheral tables and net name tags.
 * @return True if success, otherwise false.
 */
static void tick(const config &config)
{
    // Milliseconds to wait on every usb request.
    // NOTE: this cannot be forever as we need to check periodically when chimera is running
    // whether USB has been disconnected
#define USB_REQUEST_TIMEOUT_MS (1000)
    // CHIMERA Packet Format:
    // [ length low byte  | length high byte | content bytes    | ... ]

    // Get length bytes.
    // it should be sent in little endian
    uint16_t length = 0;
    for (uint8_t i = 0; i < 2; i++)
    {
        const auto out = q.pop(USB_REQUEST_TIMEOUT_MS);
        if (not out.has_value())
            return;
        reinterpret_cast<uint8_t *>(&length)[i] = out.value();
    }
    assert(length <= 64);

    // Receive content.
    std::array<uint8_t, 64> content{};
    for (uint16_t i = 0; i < length; i++)
    {
        const auto out = q.pop(USB_REQUEST_TIMEOUT_MS);
        if (not out.has_value())
            return;
        content[i] = out.value();
    }

    // Print bytes.
    for (size_t i = 0; i < length; i += 1)
        _LOG_PRINTF("%02x ", content[i]);

    // Parse content and return response.
    if (!handleContent(config, { content.data(), length }))
    {
        LOG_ERROR("Chimera: Error processing request.");
    }
}

_Noreturn void task(const config &c)
{
    osDelay(0xFFFFFFFF);

    // Main loop.
    for (;;)
    {
        // block until USB connected is ok
        hw::usb::waitForConnected();
        LOG_INFO("[CHIMERA] USB CONNECTED!");

        // For some reason that makes no sense: When I was turning on the BMS after a power cycle,
        // `hw_chimera_v2_enabled` seemed to be being set to true, which can only happen here. This makes no sense,
        // since nothing was plugged into the USB port, but I'm disabling it anyway.
        // hw_chimera_v2_enabled = true;

        // Otherwise tick.
        while (hw::usb::connected())
        {
            tick(c);
        }
        LOG_INFO("[CHIMERA] USB DISCONNECTED!");
        enabled = false;
    }
}
} // namespace chimera_v2