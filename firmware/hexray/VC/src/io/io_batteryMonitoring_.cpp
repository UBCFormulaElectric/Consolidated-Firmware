/*
DATASHEET:
https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
*/

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"

#include "io_batteryMonitoring_.hpp"

#include "io_time.hpp"
#include "util_retry.hpp"

namespace io::batteryMonitoring
{
/* -------------------- Helpers ------------------------- */
static result<void> readRegister(const uint16_t reg, const std::span<uint8_t> data)
{
    auto result = util::retry([&]() { return bat_mon.memoryRead(reg, data); }, 5);
    return result;
}
static result<void> writeRegister(const uint16_t reg, const std::span<uint8_t> data)
{
    auto result = util::retry([&]() { return bat_mon.memoryWrite(reg, data); }, 5);
    return result;
}
/* -------------------- Commands and Subcommands [Section 3.1] ------------------------- */
template <typename A, typename B> 
    requires std::same_as<A, Action>
static result<void> write(A action, uint16_t address, B data)
{
    uint8_t tx_buffer[sizeof(B)] = {};

    if constexpr (std::same_as<B, uint8_t>)
    {
        tx_buffer[0] = data;
    }
    else if constexpr (std::same_as<B, uint16_t>)
    {
        tx_buffer[0] = static_cast<uint8_t>(data & 0xFF);
        tx_buffer[1] = static_cast<uint8_t>((data >> 8) & 0xFF);
    }
    else
    {
        // Arrays (subcommand blocks), raw bytes in chip order
        std::memcpy(tx_buffer, &data, sizeof(B));
    }

    if (action == Action::COMMAND)
    {
        RETURN_IF_ERR(writeRegister(address, std::span(tx_buffer)));
    }
    else if (action == Action::SUBCOMMAND)
    {
        // The transfer buffer is 32 bytes max
        if (sizeof(B) > 32u)
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }

        uint8_t high_byte = static_cast<uint8_t>(address >> 8);
        uint8_t low_byte  = static_cast<uint8_t>(address & 0x00FF);

        // 1. Write the 16-bit subcommand address to 0x3E and 0x3F
        RETURN_IF_ERR(writeRegister(REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
        RETURN_IF_ERR(writeRegister(REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

        // 2. Write the payload data to the transfer buffer starting at 0x40
        RETURN_IF_ERR(writeRegister(REG_DATA, std::span(tx_buffer)));

        // 3. Calculate Checksum, 8-bit sum of subcommand bytes + data bytes, bitwise inverted
        uint8_t sum = low_byte + high_byte;
        for (const uint8_t byte : tx_buffer)
        {
            sum += byte;
        }

        // 4. Write checksum to 0x60 and length (data bytes + 4) to 0x61
        uint8_t check_and_len[2] = { static_cast<uint8_t>(~sum), static_cast<uint8_t>(sizeof(B) + SUBCOMMAND_BYTES) };
        // Writing length to 0x61 forces the chip to verify the checksum and save the data
        RETURN_IF_ERR(writeRegister(REG_CHECKSUM, std::span(check_and_len)));
    }
    return {};
}
template <typename A, typename B> 
    requires std::same_as<A, Action>
static result<void> read(A action, uint16_t address, B *data)
{
    *data = B{};
    uint8_t rx_buffer[sizeof(B)] = {};

    if (action == Action::COMMAND)
    {
        RETURN_IF_ERR(readRegister(address, std::span(rx_buffer)));
    }
    else if (action == Action::SUBCOMMAND)
    {
        uint8_t high_byte = static_cast<uint8_t>(address >> 8);
        uint8_t low_byte  = static_cast<uint8_t>(address & 0x00FF);

        // 1. Write lower byte of subcommand to 0x3E
        RETURN_IF_ERR(writeRegister(REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
        // 2. Write upper byte of subcommand to 0x3F
        RETURN_IF_ERR(writeRegister(REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

        // 3. Read 0x3E and 0x3F until they return what was written (0xFF means not done yet)
        uint8_t low_status   = 0xFF;
        uint8_t high_status  = 0xFF;
        bool    subcmd_ready = false;
        for (uint32_t attempt = 0; attempt < RETRIES; attempt++)
        {
            RETURN_IF_ERR(readRegister(REG_LOWER, std::span<uint8_t>(&low_status, 1)));
            RETURN_IF_ERR(readRegister(REG_UPPER, std::span<uint8_t>(&high_status, 1)));
            if ((low_status == low_byte) && (high_status == high_byte))
            {
                subcmd_ready = true;
                break;
            }
            io::time::delay(1);
        }
        if (!subcmd_ready)
        {
            return std::unexpected(ErrorCode::TIMEOUT);
        }

        // 4. Read the length of response from 0x61
        uint8_t total_len = 0;
        RETURN_IF_ERR(readRegister(REG_DATALENGTH, std::span<uint8_t>(&total_len, 1)));
        if (total_len < SUBCOMMAND_BYTES)
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }

        // 5. Read buffer starting at 0x40 for the expected length
        const uint8_t buffer_len = total_len - SUBCOMMAND_BYTES;
        if (buffer_len > sizeof(B))
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }
        if (buffer_len > 0)
        {
            RETURN_IF_ERR(readRegister(REG_DATA, std::span(rx_buffer, buffer_len)));
        }

        // 6. Read the checksum at 0x60 and verify it matches the data read
        uint8_t received_checksum = 0;
        RETURN_IF_ERR(readRegister(REG_CHECKSUM, std::span<uint8_t>(&received_checksum, 1)));
        uint8_t sum = low_byte + high_byte;
        for (uint8_t i = 0; i < buffer_len; i++)
        {
            sum += rx_buffer[i];
        }
        if (received_checksum != static_cast<uint8_t>(~sum))
        {
            return std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }
    }

    if constexpr (std::same_as<B, uint8_t>)
    {
        *data = rx_buffer[0];
    }
    else if constexpr (std::same_as<B, uint16_t>)
    {
        *data = static_cast<uint16_t>((rx_buffer[1] << 8) | rx_buffer[0]);
    }
    else
    {
        // Arrays (subcommand blocks), raw bytes in chip order
        std::memcpy(data, rx_buffer, sizeof(B));
    }
    return {};
}

/* -------------------- Voltage Readings ------------------------- */
/**
 * @brief Gets the cell voltage
 * @param cell The thing you want to read (ie: CellReading || SystemReading)
 * @return The voltage on success, errorcode if messed up
 */

template<typename A> 
    requires std::same_as<A, CellReading> || std::same_as<A, SystemReading> 
result<float> getVoltage(A request)
{
    uint16_t voltage = 0;
    RETURN_IF_ERR(read(Action::COMMAND, static_cast<uint8_t>(request), &voltage));
    if constexpr (std::same_as<A, CellReading>)
    {
        return static_cast<float>(voltage) / 1000.0f; 
    }
    else if constexpr (std::same_as<A, SystemReading>)
    {
        return static_cast<float>(voltage) / 100.0f; //  The units for TOS, PACK, and LD voltages are reported in cV (10mV LSB) by default
    }
    return {};
}

/* -------------------- Current Reading ------------------------- */
/**
 * @brief Gets the current through sense
 * @param void
 * @return float value of current
 */
result<float> getCurrent()
{
    uint16_t current = 0;
    RETURN_IF_ERR(read(Action::COMMAND, CMD_GETCURRENT, &current));
    return static_cast<float>(static_cast<int16_t>(current)) / 1000.0f;
}

/* -------------------- Temperature Reading ------------------------- */
/**
 * @brief Gets the current through sense
 * @param void
 * @return float value of current
 */
result<float> getTemperature()
{
    uint16_t temp = 0;
    RETURN_IF_ERR(read(Action::COMMAND, CMD_TEMPERATURE_IC, &temp));
    return static_cast<float>(temp) / 10.0f - 273.15f;
}

} // namespace io::batteryMonitoring