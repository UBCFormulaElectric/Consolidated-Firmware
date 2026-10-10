/*
DATASHEET:
https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
*/

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"

#include "io_batteryMonitoring_.hpp"

#include "io_batteryMonitoring_datatypes.hpp"
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
static result<void> writeRegister(const uint16_t reg, const std::span<const uint8_t> data)
{
    auto result = util::retry([&]() { return bat_mon.memoryWrite(reg, data); }, 5);
    return result;
}
/* -------------------- Commands and Subcommands [Section 3.1] ------------------------- */
static result<void> write(const Address addr, const std::span<const uint8_t> data = {})
{
    if (addr.action == Action::COMMAND)
    {
        if (data.empty()) { return std::unexpected(ErrorCode::INSUFFICENT_DATA); }
        RETURN_IF_ERR(writeRegister(addr.code, data));
    }
    else if (addr.action == Action::SUBCOMMAND)
    {
        // The transfer buffer is 32 bytes max
        if (data.size() > 32u)
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }

        uint8_t low_byte  = static_cast<uint8_t>(addr.code & 0x00FF);
        uint8_t high_byte = static_cast<uint8_t>(addr.code >> 8);

        // 1. Write the 16-bit subcommand address to 0x3E and 0x3F
        RETURN_IF_ERR(writeRegister(Communications::REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
        RETURN_IF_ERR(writeRegister(Communications::REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

        if (data.empty()) { return {}; }

        // 2. Write the payload data to the transfer buffer starting at 0x40
        RETURN_IF_ERR(writeRegister(Communications::REG_DATA, data));

        // 3. Calculate Checksum, 8-bit sum of subcommand bytes + data bytes, bitwise inverted
        uint8_t sum = low_byte + high_byte;
        for (const uint8_t byte : data)
        {
            sum += byte;
        }

        // 4. Write checksum to 0x60 and length (data bytes + 4) to 0x61
        std::array<uint8_t, 2> check_and_len = { { static_cast<uint8_t>(~sum), static_cast<uint8_t>(data.size() + Communications::SUBCOMMAND_BYTES) } };
        // Writing length to 0x61 forces the chip to verify the checksum and save the data
        RETURN_IF_ERR(writeRegister(Communications::REG_CHECKSUM, check_and_len));
    }
    return {};
}

static result<void> read(const Address addr, const std::span<uint8_t> data)
{
    std::fill(data.begin(), data.end(), 0);

    if (addr.action == Action::COMMAND)
    {
        RETURN_IF_ERR(readRegister(addr.code, data));
    }
    else if (addr.action == Action::SUBCOMMAND)
    {
        uint8_t high_byte = static_cast<uint8_t>(addr.code >> 8);
        uint8_t low_byte  = static_cast<uint8_t>(addr.code & 0x00FF);

        // 1. Write lower byte of subcommand to 0x3E
        RETURN_IF_ERR(writeRegister(Communications::REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
        // 2. Write upper byte of subcommand to 0x3F
        RETURN_IF_ERR(writeRegister(Communications::REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

        // 3. Read 0x3E and 0x3F until they return what was written (0xFF means not done yet)
        bool subcmd_ready = false;
        uint8_t low_status = 0xFF, high_status = 0xFF;
        for (size_t attempt = 0; attempt < RETRIES; attempt++)
        {
            RETURN_IF_ERR(readRegister(Communications::REG_LOWER, std::span<uint8_t>(&low_status, 1)));
            RETURN_IF_ERR(readRegister(Communications::REG_UPPER, std::span<uint8_t>(&high_status, 1)));
            if ((low_status == low_byte) && (high_status == high_byte))
            {
                subcmd_ready = true;
                break;
            }
            io::time::delay(1);
        }
        if (not subcmd_ready)
        {
            return std::unexpected(ErrorCode::TIMEOUT);
        }

        // 4. Read the length of response from 0x61
        uint8_t total_len = 0;
        RETURN_IF_ERR(readRegister(Communications::REG_DATALENGTH, std::span<uint8_t>(&total_len, 1)));
        if (total_len < Communications::SUBCOMMAND_BYTES)
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }

        // 5. Read buffer starting at 0x40 for the expected length
        const uint8_t buffer_len = total_len - Communications::SUBCOMMAND_BYTES;
        if (buffer_len > data.size())
        {
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
        }
        if (buffer_len > 0)
        {
            RETURN_IF_ERR(readRegister(Communications::REG_DATA, data.first(buffer_len)));
        }

        // 6. Read the checksum at 0x60 and verify it matches the data read
        uint8_t received_checksum = 0;
        RETURN_IF_ERR(readRegister(Communications::REG_CHECKSUM, std::span<uint8_t>(&received_checksum, 1)));
        uint8_t sum = low_byte + high_byte;
        for (size_t i = 0; i < static_cast<size_t>(buffer_len); i++)
        {
            sum += data[i];
        }
        if (received_checksum != static_cast<uint8_t>(~sum))
        {
            return std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }
    }

    return {};
}

// Overload so that we are not forced to write/read arrays, and data of scalar types. 
template <typename T>
static result<void> read(const Address addr, T& data)
{
    return read(addr, std::span<uint8_t>(reinterpret_cast<uint8_t *>(&data), sizeof(T)));
}
template <typename T>
static result<void> write(const Address addr, const T& data)
{
    return write(addr, std::span<const uint8_t>(reinterpret_cast<const uint8_t *>(&data), sizeof(T)));
}

/* -------------------- Voltage Readings ------------------------- */
template <typename T>
    requires(std::same_as<T, cellReading> || std::same_as<T, systemReading>)
result<float> getVoltage(const T request)
{
    uint16_t voltage = 0;
    RETURN_IF_ERR(read(request, voltage));
    if (std::same_as<T, cellReading>)
    {
        return static_cast<float>(voltage) / 1000.0f; // Cell voltages are reported in mV
    }
    else
    {
        return static_cast<float>(voltage) / 100.0f; //  The units for TOS, PACK, and LD voltages are reported in cV (10mV LSB) by default
    }
}
template result<float> getVoltage<cellReading>(const cellReading request);
template result<float> getVoltage<systemReading>(const systemReading request);

/* -------------------- Current Reading ------------------------- */
result<float> getCurrent()
{
    uint16_t current = 0;
    RETURN_IF_ERR(read(CURRENT_A, current));
    return static_cast<float>(static_cast<int16_t>(current)) / 1000.0f;
}

/* -------------------- Temperature Reading ------------------------- */
result<float> getTemperature()
{
    uint16_t temp = 0;
    RETURN_IF_ERR(read(DIE_TEMPERATURE_C, temp));
    return static_cast<float>(temp) / 10.0f - 273.15f;
}

/* -------------------- Initialization ------------------------- */
result<void> init()
{
    // 1. Is chip responsive
    RETURN_IF_ERR(bat_mon.isTargetReady());

    // 2.0 Check to see if chip is in DEEPSLEEP
    ControlStatus deepsleep_status{};
    uint32_t deepsleep_attempt = 0;

    RETURN_IF_ERR(read(ControlStatus::CONTROL_STATUS, deepsleep_status.raw_value));
    while (deepsleep_status.bits.DEEPSLEEP)
    {
        if (deepsleep_attempt >= RETRIES)
        {
      
            LOG_ERROR("Failed to wake device from deepsleep");
            return std::unexpected(ErrorCode::TIMEOUT);
        }
        LOG_WARN("Device in deepsleep mode");
        RETURN_IF_ERR(write(EXIT_DEEPSLEEP));
        RETURN_IF_ERR(read(ControlStatus::CONTROL_STATUS, deepsleep_status.raw_value));
        deepsleep_attempt++;
    }
    LOG_INFO("Device is out of deepsleep mode");

    // 2.1 Check to see if the device is in SLEEP
    BatteryStatus sleep_status{};
    uint32_t sleep_attempt = 0;
    RETURN_IF_ERR(read(BatteryStatus::BATTERY_STATUS, sleep_status.raw_value));
    while (sleep_status.bits.SLEEP)
    {
        if (sleep_attempt >= RETRIES)
        {
            LOG_ERROR("Failed to wake device from sleep");
            return std::unexpected(ErrorCode::TIMEOUT);
            sleep_attempt++;
        }
        LOG_WARN("Device in sleep mode");
        RETURN_IF_ERR(write(EXIT_SLEEP));
        RETURN_IF_ERR(read(BatteryStatus::BATTERY_STATUS, sleep_status.raw_value));
    }
    LOG_INFO("Device is out of sleep mode");

    // 3. Put the device into CONFIG_UPDATE mode
    RETURN_IF_ERR(write(CONFIG_UPDATE));
    BatteryStatus cfg_update{};
    bool cfg_update_rdy = false;

    for (uint32_t attempt = 0; attempt < RETRIES; attempt++)
    {
        if (read(BatteryStatus::BATTERY_STATUS, cfg_update.raw_value).has_value())
        {
            if (cfg_update.bits.CFGUPDATE)
            {
                cfg_update_rdy = true;
                LOG_INFO("Entered CONFIG_UPDATE");
                break;
            }
        }
    }
    if (not cfg_update_rdy)
    {
        LOG_ERROR("Failed to enter CONFIG_UPDATE");
        return std::unexpected(ErrorCode::TIMEOUT);
    }

    // 4. Modify internal settings

    // Four cell config
    std::array<uint8_t, 2> vcell_mode = { {0x1B, 0x00} };
    RETURN_IF_ERR(write(CONFIG_UPDATE, vcell_mode));

    // 5. Take device out of CONFIG_UPDATE mode  
    RETURN_IF_ERR(write(CONFIG_EXIT));
    LOG_INFO("Exited CONFIG_UPDATE, chip has been configured.");

    return {};
}

/* -------------------- Safety Status ------------------------- */
template <typename T>
    requires(std::same_as<T, SafetyStatusA> || std::same_as<T, SafetyStatusB>)
result<T> getSafetyStatus()
{
    T status{};
    RETURN_IF_ERR(read(T::ADDR, status));
    return status;
}
template result<SafetyStatusA> getSafetyStatus<SafetyStatusA>();
template result<SafetyStatusB> getSafetyStatus<SafetyStatusB>();
} // namespace io::batteryMonitoring