/*
DATASHEET:
https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
*/

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"

#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"
#include "io_semaphores.hpp"

#include "util_retry.hpp"

namespace io::batteryMonitoring
{
static volatile bool alert_pending = false;

// Static function declarations
static result<void> read_register(uint16_t reg, std::span<uint8_t> data);
static result<void> write_register(uint16_t reg, std::span<uint8_t> data);

static result<void> command_read_byte(uint8_t command_addr, uint8_t *data);
static result<void> command_write_byte(uint8_t command_addr, uint8_t data);
static result<void> command_read_2byte(uint8_t command_addr, uint16_t *data);
static result<void> command_write_2byte(uint8_t command_addr, uint16_t data);

static result<void> read_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static result<void> write_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static result<void> execute_subcommand(uint16_t sub_cmd);

static result<void> balancing_init();
static result<void> protection_init();
static result<void> fets_init();

/* -------------------- Helpers ------------------------- */
static result<void> read_register(const uint16_t reg, const std::span<uint8_t> data)
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    auto                       result = util::retry([&]() { return bat_mon.memoryRead(reg, data); }, 5);
    return result;
}
static result<void> write_register(const uint16_t reg, const std::span<uint8_t> data)
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    auto                       result = util::retry([&]() { return bat_mon.memoryWrite(reg, data); }, 5);
    return result;
}
/* -------------------- Commands ------------------------- */
static result<void> command_read_byte(const uint8_t command_addr, uint8_t *data)
{
    *data = 0;
    RETURN_IF_ERR_SILENT(read_register(command_addr, std::span(data, 1)));
    return {};
}
[[maybe_unused]] static result<void> command_write_byte(const uint8_t command_addr, uint8_t data)
{
    RETURN_IF_ERR_SILENT(write_register(command_addr, std::span(&data, 1)));
    return {};
}
static result<void> command_read_2byte(const uint8_t command_addr, uint16_t *data)
{
    uint8_t rx_buffer[2] = { 0, 0 };
    RETURN_IF_ERR(read_register(command_addr, std::span(rx_buffer, 2)));
    *data = static_cast<uint16_t>((rx_buffer[1] << 8) | rx_buffer[0]);
    return {};
}
static result<void> command_write_2byte(const uint8_t command_addr, const uint16_t data)
{
    uint8_t tx_buffer[2];
    tx_buffer[0] = static_cast<uint8_t>(data & 0xFF);
    tx_buffer[1] = static_cast<uint8_t>((data >> 8) & 0xFF);
    RETURN_IF_ERR_SILENT(write_register(command_addr, std::span(tx_buffer, 2)));
    return {};
}
/* -------------------- Subcommands ------------------------- */
// NOTE: Data memory access is conducted in a similar fashion to subcommands, use these helpers for data memory
[[maybe_unused]] result<void> read_subcommand(const uint16_t sub_cmd, const std::span<uint8_t> data)
{
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = static_cast<uint8_t>(sub_cmd >> 8);
    uint8_t low_byte  = static_cast<uint8_t>(sub_cmd & 0x00FF);

    // 1. Write lower byte of subcommand to 0x3E
    RETURN_IF_ERR(write_register(REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
    // 2. Write upper byte of subcommand to 0x3F
    RETURN_IF_ERR(write_register(REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

    /* 3. Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation  yet. When
    the subcommand has completed, the readback will return what was originally written. Continue reading 0x3E and 0x3F
    until it returns what was written originally */
    uint8_t low_status   = 0xFF;
    uint8_t high_status  = 0xFF;
    bool    subcmd_ready = false;
    for (uint32_t attempt = 0; attempt < RETRIES; attempt++)
    {
        RETURN_IF_ERR(read_register(REG_LOWER, std::span<uint8_t>(&low_status, 1)));
        RETURN_IF_ERR(read_register(REG_UPPER, std::span<uint8_t>(&high_status, 1)));
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
    RETURN_IF_ERR(read_register(REG_DATALENGTH, std::span<uint8_t>(&total_len, 1)));
    if (total_len < SUBCOMMAND_BYTES)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    // 5. Read buffer starting at 0x40 for the expected length
    const uint8_t buffer_len = total_len - SUBCOMMAND_BYTES;
    if (buffer_len > data.size())
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }
    if (buffer_len > 0)
    {
        RETURN_IF_ERR(read_register(REG_DATA, data.first(buffer_len)));
    }

    // 6. Read the checksum at 0x60 and verify it matches the data read.
    uint8_t received_checksum = 0;
    RETURN_IF_ERR(read_register(REG_CHECKSUM, std::span<uint8_t>(&received_checksum, 1)));

    uint16_t sum = low_byte + high_byte;
    for (const uint8_t byte : data.first(buffer_len))
    {
        sum += byte;
    }

    if (const uint8_t expected_checksum = static_cast<uint8_t>(~sum); received_checksum != expected_checksum)
    {
        return std::unexpected(ErrorCode::CHECKSUM_FAIL);
    }

    return {};
}
static result<void> write_subcommand(const uint16_t sub_cmd, const std::span<uint8_t> data)
{
    // The transfer buffer is 32 bytes max
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = static_cast<uint8_t>(sub_cmd >> 8);
    uint8_t low_byte  = static_cast<uint8_t>(sub_cmd & 0x00FF);

    // 1. Write the 16-bit subcommand address to 0x3E and 0x3F
    RETURN_IF_ERR(write_register(REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
    RETURN_IF_ERR(write_register(REG_UPPER, std::span<uint8_t>(&high_byte, 1)));

    // 2. Write the payload data to the transfer buffer starting at 0x40
    if (data.size() > 0)
    {
        RETURN_IF_ERR(write_register(REG_DATA, data));
    }

    // 3. Calculate Checksum, 8-bit sum of subcommand bytes + data bytes
    uint8_t sum = low_byte + high_byte;
    for (const uint8_t byte : data)
    {
        sum += byte;
    }
    const uint8_t checksum = static_cast<uint8_t>(~sum); // Bitwise invert the 8-bit sum

    // 4. Calculate Data Length, length = data payload bytes + 4 (for 0x3E, 0x3F, 0x60, 0x61)
    const uint8_t total_len = static_cast<uint8_t>(data.size() + 4);

    // 5. Combine Checksum and Length into a single 16-bit word, checksum goes to 0x60 (lower byte), length goes to 0x61
    const uint16_t check_and_len = static_cast<uint16_t>((total_len << 8) | checksum);

    // writing length to 0x61 forces the chip to verify the checksum and save the data
    RETURN_IF_ERR(command_write_2byte(REG_CHECKSUM, check_and_len));

    return {};
}
static result<void> execute_subcommand(const uint16_t sub_cmd)
{
    uint8_t low_byte  = static_cast<uint8_t>(sub_cmd & 0x00FF);
    uint8_t high_byte = static_cast<uint8_t>(sub_cmd >> 8);
    RETURN_IF_ERR(write_register(REG_LOWER, std::span<uint8_t>(&low_byte, 1)));
    RETURN_IF_ERR(write_register(REG_UPPER, std::span<uint8_t>(&high_byte, 1)));
    return {};
}

/* -------------------- Voltage Readings ------------------------- */
/**
 * @brief Gets the cell voltage
 * @param cell The cell you want to read (ie: CellReading::CELL1)
 * @return The voltage on success, erorcode if messed up
 */
result<float> get_voltage_cell(CellReading cell)
{
    uint16_t cell_voltage = 0;
    RETURN_IF_ERR(command_read_2byte(static_cast<uint8_t>(cell), &cell_voltage));
    return static_cast<float>(cell_voltage) / 1000.0f;
}
/**
 * @brief Gets the system voltage
 * @param system The thing u want to read (ie: SystemReding::Pack)
 * @return The voltage on success, erorcode if messed up
 */
result<float> get_voltage_system(const SystemReading system)
{
    uint16_t system_voltage = 0;
    RETURN_IF_ERR(command_read_2byte(static_cast<uint8_t>(system), &system_voltage));
    return static_cast<float>(system_voltage) /
           100.0f; //  The units for TOS, PACK, and LD voltages are reported in cV (10mV LSB) by default
}
/* -------------------- Current Readings ------------------------- */
/**
 * @brief Gets the current through sense
 * @param void
 * @return float value of current
 */
result<float> get_current()
{
    uint16_t raw_current = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_GETCURRENT, &raw_current));
    const int16_t signed_current = static_cast<int16_t>(raw_current);
    return static_cast<float>(signed_current) / 1000.0f;
}

/* -------------------- Charge Readings ------------------------- */
// I'm not 100% sure about the purpose of this yet, ik its important for SOC but idk past that
result<uint64_t> get_integrated_charge()
{
    std::array<uint8_t, 8> integrated_charge;
    RETURN_IF_ERR(read_subcommand(SUBCMD_GET_INEGRATED_CHARGE, integrated_charge));
    uint64_t reading = 0;
    std::memcpy(&reading, integrated_charge.data(), sizeof(reading));
    return reading;
}

/* -------------------- Cell Balancing ------------------------- */
/**
 * @brief Gets the temperature of the die because we will be using autonomous balancing
 * @param void
 * @return float value of current
 */
result<float> get_temperatureIC()
{
    uint16_t raw_temp = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_TEMPERATURE_IC, &raw_temp));
    const float temp_kelvin  = static_cast<float>(raw_temp) / 10.0f;
    float       temp_celsius = temp_kelvin - 273.15f;
    return temp_celsius;
}
/**
 * @brief Initialization of cell balancing (intervals, temperature limits, allowed amounts, start conditions)
 * (Sections 10.1 and 10.2, pg. 75)
 * @param void
 */
[[maybe_unused]] static result<void> balancing_init()
{
    uint8_t cell_balancing = 0x00;
    RETURN_IF_ERR(write_subcommand(BALANCE_CFG, std::span<uint8_t>(&cell_balancing, 1)));

    uint8_t min_temprature_cell = 0x80;
    RETURN_IF_ERR(write_subcommand(0x9336, std::span<uint8_t>(&min_temprature_cell, 1)));

    uint8_t max_temprature_cell = 0x7F;
    RETURN_IF_ERR(write_subcommand(0x9337, std::span<uint8_t>(&max_temprature_cell, 1)));

    uint8_t max_internal_temp = 0x7F;
    RETURN_IF_ERR(write_subcommand(MAX_IC_TEMP, std::span<uint8_t>(&max_internal_temp, 1)));

    uint8_t balance_interval = 0x14;
    RETURN_IF_ERR(write_subcommand(CELL_BALANCE_INTERVAL, std::span<uint8_t>(&balance_interval, 1)));

    uint8_t max_cells = 0x02;
    RETURN_IF_ERR(write_subcommand(MAX_CELLS_BALANCING, std::span<uint8_t>(&max_cells, 1)));

    std::array<uint8_t, 2> min_voltage_charge = { { 0x00, 0x00 } }; // TODO: set to 0x09C4 = 2500 mV after testing...
    RETURN_IF_ERR(write_subcommand(0x933B, min_voltage_charge));

    std::array<uint8_t, 2> min_voltage_delta = { { 0x00, 0x00 } };
    RETURN_IF_ERR(write_subcommand(0x933D, min_voltage_delta));

    std::array<uint8_t, 2> min_voltage_relax = { { 0x00, 0x00 } }; // TODO: set to 20mV after testing...
    RETURN_IF_ERR(write_subcommand(0x933F, min_voltage_relax));

    return {};
}
/**
 * @brief Related to manual cell balancing, sending a subcommand to manually balance.
 * @param cell the cell you wish you balance (etc: CellBalance_BitMask::CELL1)
 */
result<void> send_balancing_subcommand(CellBalance_BitMask cell)
{
    std::array<uint8_t, 2> cell_to_balance = { { static_cast<uint8_t>(static_cast<uint16_t>(cell) & 0xFF),
                                                 static_cast<uint8_t>(static_cast<uint16_t>(cell) >> 8) } };
    RETURN_IF_ERR(write_subcommand(CB_ACTIVE_CELLS, cell_to_balance));
    return {};
}
/**
 * @brief Reads what cells are being balanced
 * @param void
 * @return uint16_t a mask of the cells that are being balanced.
 */
result<uint16_t> read_balancing_subcommand()
{
    std::array<uint8_t, 2> buf = { { 0, 0 } };
    if (const auto err = read_subcommand(CB_ACTIVE_CELLS, buf); !err)
        return std::unexpected(err.error());
    const uint16_t mask = static_cast<uint16_t>(static_cast<uint16_t>(buf[0]) | (static_cast<uint16_t>(buf[1]) << 8));
    return mask;
}
/**
 * @brief Manually balances every cell that is over a certain voltage
 * @param threshold_mV the threshold in mV you want to surpass
 */
result<void> send_balancing_above_threshold(const uint16_t threshold_mV)
{
    std::array<uint8_t, 2> payload = { { static_cast<uint8_t>(threshold_mV & 0xFF),
                                         static_cast<uint8_t>((threshold_mV >> 8) & 0xFF) } };
    RETURN_IF_ERR(write_subcommand(0x0084, payload));
    return {};
}
result<void> stop_balancing_subcommand()
{
    std::array<uint8_t, 2> cell_to_balance = { { 0x00, 0x00 } };
    RETURN_IF_ERR(write_subcommand(CB_ACTIVE_CELLS, cell_to_balance));
    return {};
}

/* -------------------- Read Status/Alarm Registers ------------------------- */

result<int16_t> read_currentcc1()
{
    std::array<uint8_t, 32> buf{};
    RETURN_IF_ERR(read_subcommand(0x0075, buf));

    int16_t cc1 = static_cast<int16_t>(static_cast<uint16_t>(buf[22]) | (static_cast<uint16_t>(buf[23]) << 8));

    return cc1;
}

result<uint16_t> balacing_time_elapsed()
{
    std::array<uint8_t, 2> buf = { { 0, 0 } };
    RETURN_IF_ERR(read_subcommand(CBSTATUS1, buf));
    return static_cast<uint16_t>(static_cast<uint16_t>(buf[0]) | (static_cast<uint16_t>(buf[1]) << 8));
}

result<AlertStatus> read_alarm_status()
{
    AlertStatus status{};
    RETURN_IF_ERR(command_read_2byte(CMD_ALARM_STATUS, &status.raw_value));
    return status;
}
/* -------------------- Protections ------------------------- */
/**
 * @brief Initializing Protections
 * @param void
 */
[[maybe_unused]] static result<void> protection_init()
{
    // 1. ALERT Pin Configuration
    uint8_t alert = 0x82; // maybe 0x02
    RETURN_IF_ERR(write_subcommand(ALERT, std::span<uint8_t>(&alert, 1)));

    // 2. Disable all permanent faults
    std::array<uint8_t, 2> protections = { { 0x00, 0x00 } };
    RETURN_IF_ERR(write_subcommand(MFG_STATUS_INIT, protections));
    uint8_t PF_protections = 0x00;
    RETURN_IF_ERR(write_subcommand(0x9261, std::span<uint8_t>(&PF_protections, 1)));
    RETURN_IF_ERR(write_subcommand(0x92C4, std::span<uint8_t>(&PF_protections, 1)));
    RETURN_IF_ERR(write_subcommand(0x92C5, std::span<uint8_t>(&PF_protections, 1)));

    // 3. Initialize what protections
    uint8_t SF_protectionsA = 0x0C; // CUV/COV
    uint8_t SF_protectionsB = 0x40; // Internal Die OT
    RETURN_IF_ERR(write_subcommand(REG_SF_ALERT_MASK_A, std::span<uint8_t>(&SF_protectionsA, 1)));
    RETURN_IF_ERR(write_subcommand(REG_SF_ALERT_MASK_B, std::span<uint8_t>(&SF_protectionsB, 1)));

    // 4. Write OV/UV
    uint8_t overvoltage  = 0x53;
    uint8_t undervoltage = 0x32;
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_COV, std::span<uint8_t>(&overvoltage, 1)));
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_CUV, std::span<uint8_t>(&undervoltage, 1)));

    // 5. Configure Default Alarm Mask to map SSA and SSBC to the ALERT pin
    std::array<uint8_t, 2> alarm_mask = { { 0x00, 0x0C } };
    RETURN_IF_ERR(write_subcommand(DEFAULT_ALARM, alarm_mask));

    return {};
}

result<std::array<uint16_t, 5>> get_voltage_UV(const uint16_t sub_cmd)
{
    CUV                     cuv{};
    std::array<uint16_t, 5> parsed_snapshot_voltages{};
    RETURN_IF_ERR(read_subcommand(sub_cmd, cuv.snapshot_undervoltages));

    parsed_snapshot_voltages[0] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cuv.snapshot_undervoltages[1]) << 8) | cuv.snapshot_undervoltages[0]);
    parsed_snapshot_voltages[1] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cuv.snapshot_undervoltages[3]) << 8) | cuv.snapshot_undervoltages[2]);
    parsed_snapshot_voltages[2] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cuv.snapshot_undervoltages[5]) << 8) | cuv.snapshot_undervoltages[4]);
    parsed_snapshot_voltages[3] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cuv.snapshot_undervoltages[7]) << 8) | cuv.snapshot_undervoltages[6]);
    parsed_snapshot_voltages[4] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cuv.snapshot_undervoltages[9]) << 8) | cuv.snapshot_undervoltages[8]);

    return parsed_snapshot_voltages;
}

result<std::array<uint16_t, 5>> get_voltage_OV(const uint16_t sub_cmd)
{
    COV                     cov{};
    std::array<uint16_t, 5> parsed_snapshot_voltages{};
    RETURN_IF_ERR(read_subcommand(sub_cmd, cov.snapshot_overvoltages));

    parsed_snapshot_voltages[0] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cov.snapshot_overvoltages[1]) << 8) | cov.snapshot_overvoltages[0]);
    parsed_snapshot_voltages[1] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cov.snapshot_overvoltages[3]) << 8) | cov.snapshot_overvoltages[2]);
    parsed_snapshot_voltages[2] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cov.snapshot_overvoltages[5]) << 8) | cov.snapshot_overvoltages[4]);
    parsed_snapshot_voltages[3] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cov.snapshot_overvoltages[7]) << 8) | cov.snapshot_overvoltages[6]);
    parsed_snapshot_voltages[4] = static_cast<uint16_t>(
        (static_cast<uint16_t>(cov.snapshot_overvoltages[9]) << 8) | cov.snapshot_overvoltages[8]);

    return parsed_snapshot_voltages;
}
/**
 * @brief ISR Handler
 * @param void
 */
void alert_handler()
{
    alert_pending = true;
}
bool consume_alert_pending()
{
    taskENTER_CRITICAL();
    const bool was_pending = alert_pending;
    alert_pending          = false;
    taskEXIT_CRITICAL();
    return was_pending;
}

result<SafetyStatusA> get_safety_alert_a()
{
    SafetyStatusA status{};
    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_STATUS_A, &status.raw_status));
    return status;
}
result<SafetyStatusB> get_safety_alert_b()

{
    SafetyStatusB status{};
    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_STATUS_B, &status.raw_status));
    return status;
}

[[maybe_unused]] result<void> fets_init()
{
    RETURN_IF_ERR(execute_subcommand(SUBCMD_ALL_FETS_ON)); // 0x0096
    LOG_INFO("FETs enabled");
    return {};
}

result<void> init()
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    // 1. Is chip responsive
    RETURN_IF_ERR(bat_mon.isTargetReady());

    // 2.0 Check to see if chip is in DEEPSLEEP
    ControlStatus control_status{};
    uint32_t      deepsleep_attempt = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_CONTROL_STATUS, &control_status.raw_value));
    while (control_status.bits.DEEPSLEEP)
    {
        if (deepsleep_attempt >= RETRIES)
        {
            LOG_ERROR("Failed to wake device from deepsleep");
            return std::unexpected(ErrorCode::TIMEOUT);
            deepsleep_attempt++;
        }
        LOG_WARN("Device in deepsleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_DEEPSLEEP));
        RETURN_IF_ERR(command_read_2byte(CMD_CONTROL_STATUS, &control_status.raw_value));
    }
    LOG_INFO("Device is out of deepsleep mode");

    // 2.1 Check to see if the device is in SLEEP
    BatteryStatus battery_status{};
    uint32_t      sleep_attempt = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_BATTERY_STATUS, &battery_status.raw_value));
    while (battery_status.bits.SLEEP)
    {
        if (sleep_attempt >= RETRIES)
        {
            LOG_ERROR("Failed to wake device from sleep");
            return std::unexpected(ErrorCode::TIMEOUT);
            sleep_attempt++;
        }
        LOG_WARN("Device in sleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_SLEEP));
        RETURN_IF_ERR(command_read_2byte(CMD_BATTERY_STATUS, &battery_status.raw_value));
    }
    LOG_INFO("Device is out of sleep mode");

    // 3. Put the device into CONFIG_UPDATE mode
    RETURN_IF_ERR(execute_subcommand(SUBCMD_SET_CFGUPDATE));
    BatteryStatus cfgupdate{};
    bool          cfgupdate_ready = false;

    for (uint32_t attempt = 0; attempt < RETRIES; attempt++)
    {
        if (command_read_2byte(CMD_BATTERY_STATUS, &cfgupdate.raw_value).has_value())
        {
            if (cfgupdate.bits.CFGUPDATE)
            {
                cfgupdate_ready = true;
                LOG_INFO("Entered CONFIG_UPDATE");
                break;
            }
        }
    }
    if (!cfgupdate_ready)
    {
        LOG_ERROR("Failed to enter CONFIG_UPDATE");
        return std::unexpected(ErrorCode::TIMEOUT);
    }

    // 4. Modify settings

    // Protections
    RETURN_IF_ERR(protection_init());

    // VCELL mode
    std::array<uint8_t, 2> vcell_mode = { { 0x1B, 0x00 } };
    RETURN_IF_ERR(write_subcommand(VCELL_MODE, vcell_mode));

    // Balancing
    RETURN_IF_ERR(balancing_init());

    // 4. Take device out of configuration mode
    RETURN_IF_ERR(execute_subcommand(EXIT_CFGUPDATE));
    LOG_INFO("Exited CONFIG_UPDATE");

    // 5. Must do this because init will need to be called whenever GLVMS has been switched off
    RETURN_IF_ERR(execute_subcommand(SUBCMD_RESETCHARGEACCUM));

    // fets_init();

    return {};
}
} // namespace io::batteryMonitoring

// FUNCTIONS THAT COULD BE OF USE NEXT YEAR...
/*
std::expected<uint32_t, ErrorCode> raw_voltages_and_currents(CellNum cell, Measurement measurement_type)
{
    uint16_t subcmd = 0;

    if (cell == CELL5)
    {
        subcmd = CMD_V_C_COUNT2;
    }
    else
    {
        subcmd = CMD_V_C_COUNT1;
    }

    std::array<uint8_t, 32> data{};
    RETURN_IF_ERR(read_subcommand(subcmd, data));

    // Each cell block is 8 bytes: first 4 bytes voltage, next 4 bytes current.
    uint32_t cell_start = 0;
    if (cell != CELL5)
    {
        cell_start = (static_cast<uint32_t>(cell) - 1u) * 8u;
    }

    uint32_t value_start = cell_start;

    if (measurement_type == CURRENT)
    {
        value_start = cell_start + 4u;
    }

    uint32_t reading = static_cast<uint32_t>(data[value_start]) | (static_cast<uint32_t>(data[value_start + 1]) << 8) |
                       (static_cast<uint32_t>(data[value_start + 2]) << 16) |
                       (static_cast<uint32_t>(data[value_start + 3]) << 24);

    return reading;
}

// TODO: OTP configuration
std::expected<void, ErrorCode> OTP(void)
{
    // check if OTP can be written to, the device must be in FULLACCESS and CONFIG_UPDATE mode when this subcommand is
    // sent.

    // CONFIG MODE is only available in FULL ACCESS MODE, which can only be accessed if we transition from SEALED to
    // UNSEALED mode SEALED -> UNSEALED -> FULLACCESS -> FULL CONFIG MODE

    // CRC = x^8 + x^2 + x + 1
    //  std::span<uint8_t> otp_programming_ok;
    //  read_subcommand(OTP_WR_CHECK, otp_programming_ok);

    return {};
}

*/

/*SECURITY MODES (for OTP)
uint16_t security_state_unfiltered = 0;
RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state_unfiltered));
SecurityState security_state = static_cast<SecurityState>((security_state_unfiltered & 0x0300u) >> 8u);

 uint16_t full_access = 0x1100; idk anything but 0xFFFF
std::array<uint8_t, 2> full_access_bytes = {{static_cast<uint8_t>(full_access & 0x00FF),
static_cast<uint8_t>((full_access >> 8) & 0x00FF)}};

RETURN_IF_ERR(write_data_memory(FULL_ACCESS_EDIT, full_access_bytes));

Lowkey might be unneccesary due to the fact that there is no direct mention of how the chip boots up, but it does
give the option to transsniton to sealed mode, so ig the assumption can be made that we begin in FULL ACCESS mode
if (security_state == SecurityState::SEALED)
{
    RETURN_IF_ERR(write_register_byte(REG_LOWER, static_cast<uint8_t>(SECURITY_UNSEAL_FIRST)));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, static_cast<uint8_t>(SECURITY_UNSEAL_FIRST >> 8)));
    RETURN_IF_ERR(write_register_byte(REG_LOWER, static_cast<uint8_t>(SECURITY_UNSEAL_SECOND)));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, static_cast<uint8_t>(SECURITY_UNSEAL_SECOND >> 8)));
    // io::time::delay(10);

    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state_unfiltered));
    security_state = static_cast<SecurityState>((security_state_unfiltered & 0x0300u) >> 8u);
    if (security_state == SecurityState::SEALED)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
}

else if (security_state == SecurityState::UNSEALED)
{
    This is very suspicious way of doing this, prior to OTP ensure to review because the security codes cannot be
     the default 0xFFFF. If tests work and I check via debugging that the shit jumps straight into FULL ACESS on
     boot, I can js get rid of this and edit the data memory in init() and js boot at FULL ACESSS alwats
    RETURN_IF_ERR(write_register_byte(REG_LOWER, static_cast<uint8_t>(SECURITY_FULLACESS)));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, static_cast<uint8_t>(SECURITY_FULLACESS >> 8)));
    RETURN_IF_ERR(write_register_byte(REG_LOWER, static_cast<uint8_t>(SECURITY_FULLACESS)));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, static_cast<uint8_t>(SECURITY_FULLACESS >> 8)));
    RETURN_IF_ERR(write_subcommand(full_access, {}));
    io::time::delay(10);

    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state_unfiltered));
    security_state = static_cast<SecurityState>((security_state_unfiltered & 0x0300u) >> 8u);
    if (security_state != SecurityState::FULL_ACCESS)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
} */
