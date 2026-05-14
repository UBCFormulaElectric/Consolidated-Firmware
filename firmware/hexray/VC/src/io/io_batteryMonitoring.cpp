/*
DATASHEET:
https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
*/

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"
#include "io_batteryCharging.hpp"

namespace io::batteryMonitoring
{

// Static function declarations
static std::expected<void, ErrorCode> read_register(uint16_t reg, std::span<uint8_t> data);
static std::expected<void, ErrorCode> write_register(uint16_t reg, std::span<uint8_t> data);

static std::expected<void, ErrorCode> command_read_byte(uint8_t command_addr, uint8_t *data);
static std::expected<void, ErrorCode> command_write_byte(uint8_t command_addr, uint8_t data);
static std::expected<void, ErrorCode> command_read_2byte(uint8_t command_addr, uint16_t *data);
static std::expected<void, ErrorCode> command_write_2byte(uint8_t command_addr, uint16_t data);

static std::expected<void, ErrorCode> read_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static std::expected<void, ErrorCode> write_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static std::expected<void, ErrorCode> execute_subcommand(uint16_t sub_cmd);

static std::expected<void, ErrorCode> balancing_init();
static std::expected<void, ErrorCode> protection_init();
static std::expected<void, ErrorCode> fetsInit();

/* -------------------- Helpers ------------------------- */
static std::expected<void, ErrorCode> read_register(uint16_t reg, std::span<uint8_t> data)
{
    RETURN_IF_ERR_SILENT(bat_mon.memoryRead(reg, data));
    return {};
}
static std::expected<void, ErrorCode> write_register(uint16_t reg, std::span<uint8_t> data)
{
    RETURN_IF_ERR_SILENT(bat_mon.memoryWrite(reg, data));
    return {};
}

/* -------------------- Commands ------------------------- */
[[maybe_unused]] static std::expected<void, ErrorCode> command_read_byte(uint8_t command_addr, uint8_t *data)
{
    *data = 0;
    RETURN_IF_ERR_SILENT(read_register(command_addr, std::span(data, 1)));
    return {};
}
[[maybe_unused]] static std::expected<void, ErrorCode> command_write_byte(uint8_t command_addr, uint8_t data)
{
    RETURN_IF_ERR_SILENT(write_register(command_addr, std::span(&data, 1)));
    return {};
}
static std::expected<void, ErrorCode> command_read_2byte(uint8_t command_addr, uint16_t *data)
{
    uint8_t rx_buffer[2] = { 0, 0 };
    RETURN_IF_ERR(read_register(command_addr, std::span(rx_buffer, 2)));
    *data = static_cast<uint16_t>((rx_buffer[1] << 8) | rx_buffer[0]);
    return {};
}
static std::expected<void, ErrorCode> command_write_2byte(uint8_t command_addr, uint16_t data)
{
    uint8_t tx_buffer[2];
    tx_buffer[0] = static_cast<uint8_t>(data & 0xFF);
    tx_buffer[1] = static_cast<uint8_t>((data >> 8) & 0xFF);
    RETURN_IF_ERR_SILENT(write_register(command_addr, std::span(tx_buffer, 2)));
    return {};
}

/* -------------------- Subcommands ------------------------- */
// NOTE: Data memory access is conducted in a similar fashion to subcommands, use these helpers for data memory
[[maybe_unused]] std::expected<void, ErrorCode> read_subcommand(uint16_t sub_cmd, std::span<uint8_t> data)
{
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = (uint8_t)(sub_cmd >> 8);
    uint8_t low_byte  = (uint8_t)(sub_cmd & 0x00FF);

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
    uint8_t buffer_len = total_len - SUBCOMMAND_BYTES;
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
    for (uint8_t byte : data.first(buffer_len))
    {
        sum += byte;
    }

    uint8_t expected_checksum = static_cast<uint8_t>(~sum);

    if (received_checksum != expected_checksum)
    {
        return std::unexpected(ErrorCode::CHECKSUM_FAIL);
    }

    return {};
}
static std::expected<void, ErrorCode> write_subcommand(uint16_t sub_cmd, std::span<uint8_t> data)
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
    for (uint8_t byte : data)
    {
        sum += byte;
    }
    uint8_t checksum = ~sum; // Bitwise invert the 8-bit sum

    // 4. Calculate Data Length, length = data payload bytes + 4 (for 0x3E, 0x3F, 0x60, 0x61)
    uint8_t total_len = static_cast<uint8_t>(data.size() + 4);

    // 5. Combine Checksum and Length into a single 16-bit word, checksum goes to 0x60 (lower byte), length goes to 0x61
    // (upper byte)
    uint16_t check_and_len = static_cast<uint16_t>((total_len << 8) | checksum);

    // writing length to 0x61 forces the chip to verify the checksum and save the data
    RETURN_IF_ERR(command_write_2byte(REG_CHECKSUM, check_and_len));

    return {};
}
static std::expected<void, ErrorCode> execute_subcommand(uint16_t sub_cmd)
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
 * @param CellReading The cell you want to read (ie: CellReading::CELL1)
 * @return The voltage on success, erorcode if messed up
 */
std::expected<float, ErrorCode> get_voltage_cell(CellReading cell)
{
    uint16_t cell_voltage = 0;
    RETURN_IF_ERR(command_read_2byte((uint8_t)cell, &cell_voltage));
    return static_cast<float>(cell_voltage) / 1000.0f;
}
/**
 * @brief Gets the system voltage
 * @param SystemReading The thing u want to read (ie: SystemReding::Pack)
 * @return The voltage on success, erorcode if messed up
 */
std::expected<float, ErrorCode> get_voltage_system(SystemReading system)
{
    uint16_t system_voltage = 0;
    RETURN_IF_ERR(command_read_2byte(system, &system_voltage));
    return static_cast<float>(system_voltage) /
           100.0f; //  The units for TOS, PACK, and LD voltages are reported in cV (10mV LSB) by default
}
/* -------------------- Current Readings ------------------------- */
/**
 * @brief Gets the current through sense
 * @param void
 * @return float value of current
 */
std::expected<float, ErrorCode> get_current()
{
    uint16_t raw_current = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_GETCURRENT, &raw_current));
    int16_t signed_current = static_cast<int16_t>(raw_current);
    return static_cast<float>(signed_current) / 1000.0f;
}

/* -------------------- Charge Readings ------------------------- */
// I'm not 100% sure about the purpose of this yet, ik its important for SOC but idk past that
std::expected<uint64_t, ErrorCode> get_integrated_charge()
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
std::expected<float, ErrorCode> get_temperatureIC()
{
    uint16_t raw_temp = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_TEMPERATURE_IC, &raw_temp));
    float temp_kelvin  = static_cast<float>(raw_temp) / 10.0f;
    float temp_celsius = temp_kelvin - 273.15f;
    return temp_celsius;
}
/**
 * @brief initialization of cell balancing
 * @param void
 * @return no
 */
[[maybe_unused]] static std::expected<void, ErrorCode> balancing_init()
{
    // 1. Configure device to handle balancing itself
    uint8_t cell_balancing = 0x03;
    RETURN_IF_ERR(write_subcommand(BALANCE_CFG, std::span<uint8_t>(&cell_balancing, 1)));

    // 2. Die internal temperature 
    uint8_t max_internal_temp = 70;
    RETURN_IF_ERR(write_subcommand(MAX_IC_TEMP, std::span<uint8_t>(&max_internal_temp, 1)));

    // 3. Balance time
    uint8_t balance_interval = 20;
    RETURN_IF_ERR(write_subcommand(CELL_BALANCE_INTERVAL, std::span<uint8_t>(&balance_interval, 1)));

    uint8_t max_cells = 1;
    RETURN_IF_ERR(write_subcommand(MAX_CELLS_BALANCING, std::span<uint8_t>(&max_cells, 1)));

    // 2. Min/Max voltage
    std::array<uint8_t, 2> min_voltage = {{0x10, 0x0C}}; //0x0C10
    RETURN_IF_ERR(write_subcommand(CELL_BALANCE_MIN_V, min_voltage));

    return {};
}

std::expected<void, ErrorCode> send_balancing_subcommand(CellBalance_BitMask cell)
{
    std::array<uint8_t, 2> cell_to_balance= {{(uint8_t)((uint16_t)cell & 0xFF), (uint8_t)((uint16_t)cell >> 8)}};
    write_subcommand(CB_ACTIVE_CELLS, cell_to_balance);
    return {};
}

std::expected<void, ErrorCode> stop_balancing_subcommand()
{
    std::array<uint8_t, 2> cell_to_balance= {{0x00, 0x00}};
    write_subcommand(CB_ACTIVE_CELLS, cell_to_balance);
    return {};
}

std::expected<bool, ErrorCode> is_balancing_active()
{
    uint16_t alarm_status = 0;
    RETURN_IF_ERR(command_read_2byte(0x62, &alarm_status));

    bool cb_alarm = (alarm_status & 0x0004);

    std::array<uint8_t, 2> buf{};
    RETURN_IF_ERR(read_subcommand(CB_ACTIVE_CELLS, buf));

    bool balancing_active = ((buf[0] | (buf[1] << 8)) != 0);

    LOG_INFO("AlarmStatus=0x%04X, CB bit=%s, CB_ACTIVE_CELLS=[0x%02X 0x%02X], active=%s",
             alarm_status,
             cb_alarm ? "true" : "false",
             buf[0],
             buf[1],
             balancing_active ? "true" : "false");
    
    std::array<uint8_t, 1> readback{};
    read_subcommand(BALANCE_CFG, readback);
    LOG_INFO("BALANCE_CFG readback: 0x%02X", readback[0]);

    return balancing_active;
}

/* -------------------- Protections ------------------------- */
/**
 * @brief Init OV/UV protections
 * @param void
 * @return 
 */
[[maybe_unused]]static std::expected<void, ErrorCode> protection_init()
{
    // 1. Initialize what protections
    uint8_t protectionsA = 0x0C;
    uint8_t protectionsB = 0x20;
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_A, std::span<uint8_t>(&protectionsA, 1)));
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_B, std::span<uint8_t>(&protectionsB, 1)));

    // 2. Write OV/UV
    uint8_t overvoltage = 0x53;
    uint8_t undervoltage = 0x31;
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_COV, std::span<uint8_t>(&overvoltage, 1)));
    RETURN_IF_ERR(write_subcommand(REG_PROTECTIONS_CUV, std::span<uint8_t>(&undervoltage, 1)));

    // 3. Soft alerts
    uint8_t sfAlertMaskA = 0x0C;
    RETURN_IF_ERR(write_subcommand(REG_SF_ALERT_MASK_A, std::span<uint8_t>(&sfAlertMaskA, 1)));

    return {};
}

/**
 * @brief Gets Safety Alert A register
 * @param void
 * @return uint8_t value of safety alert A
 */
std::expected<uint8_t, ErrorCode> get_safety_alert_a()
{
    uint8_t reading = 0;
    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_ALERT_A, &reading));
    return reading;
}

/**
 * @brief Gets Safety Status A register
 * @param void
 * @return uint8_t value of safety status A
 */
std::expected<uint8_t, ErrorCode> get_safety_status_a()
{
    uint8_t reading = 0;
    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_STATUS_A, &reading));
    return reading;
}

/**
 * @brief Checks whether cell overvoltage or undervoltage fault is active
 * @param void
 * @return true if OV or UV fault is active, false otherwise
 */
std::expected<bool, ErrorCode> is_cell_ov_uv_fault_active()
{
    uint8_t safety_alert_a  = 0;
    uint8_t safety_status_a = 0;

    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_ALERT_A, &safety_alert_a));
    RETURN_IF_ERR(command_read_byte(CMD_SAFETY_STATUS_A, &safety_status_a));

    bool ov_fault = ((safety_alert_a & SAFETY_A_COV) != 0u) || ((safety_status_a & SAFETY_S_COV) != 0u);
    bool uv_fault = ((safety_alert_a & SAFETY_A_CUV) != 0u) || ((safety_status_a & SAFETY_S_CUV) != 0u);

    return (ov_fault || uv_fault);
}

[[maybe_unused]] std::expected<void, ErrorCode> fetsInit()
{
    RETURN_IF_ERR(execute_subcommand(SUBCMD_ALL_FETS_ON)); // 0x0096
    LOG_INFO("FETs enabled");
    return {};
}

std::expected<void, ErrorCode> init()
{
    // 1. Is chip responsive
    RETURN_IF_ERR(bat_mon.isTargetReady());

    // 2.0 Check to see if chip is in DEEPSLEEP
    uint16_t control_status = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_CONTROL_STATUS, &control_status));
    while (control_status & CTRL_STATUS_DEEPSLEEP)
    {
        LOG_WARN("Device in deepsleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_DEEPSLEEP));
        RETURN_IF_ERR(command_read_2byte(CMD_CONTROL_STATUS, &control_status));
    }
    LOG_INFO("Device is out of deepsleep mode");

    // 2.1 Check to see if the device is in SLEEP
    uint16_t battery_status = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_BATTERY_STATUS, &battery_status));
    while (battery_status & BAT_STATUS_SLEEP)
    {
        LOG_WARN("Device in sleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_SLEEP));
        RETURN_IF_ERR(command_read_2byte(CMD_BATTERY_STATUS, &battery_status));
    }
    LOG_INFO("Device is out of sleep mode");

    // 3. Put the device into CONFIG_UPDATE mode
    RETURN_IF_ERR(execute_subcommand(SUBCMD_SET_CFGUPDATE));
    constexpr uint16_t BAT_STATUS_CFGUPDATE = (1 << 0);

    uint16_t cfgupdate       = 0;
    bool     cfgupdate_ready = false;

    for (uint32_t attempt = 0; attempt < RETRIES; attempt++)
    {
        if (command_read_2byte(CMD_BATTERY_STATUS, &cfgupdate).has_value())
        {
            if ((cfgupdate & BAT_STATUS_CFGUPDATE) != 0)
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
    //RETURN_IF_ERR(protection_init());

    // ALERT
    uint8_t alert = 0x82; //maybe 0x02
    RETURN_IF_ERR(write_subcommand(ALERT, std::span<uint8_t>(&alert, 1)));

    // VCELL mode
    std::array <uint8_t, 2> vcell_mode = {{0x1B, 0x00}};
    RETURN_IF_ERR(write_subcommand(VCELL_MODE, vcell_mode));

    // Balancing
    RETURN_IF_ERR(balancing_init());

    // Add one that edits the security registers (OTP SHIT)

    // 4. Take device out of configuration mode
    RETURN_IF_ERR(execute_subcommand(EXIT_CFGUPDATE));

    // 5. Must do this because init will need to be called whenever GLVMS has been switched off
    RETURN_IF_ERR(execute_subcommand(SUBCMD_RESETCHARGEACCUM));

    // fetsInit();

    return {};
}

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

// Can clean up to whichever ones we want later, for now I simply left with all of them. Talk to ELEC
std::expected<void, ErrorCode> check_safety_status()
{
    // Read Alarm Status (0x62) — SSA and SSBC summary bits
    uint16_t alarm_status = 0;
    RETURN_IF_ERR(read_register_word(REG_ALARM_STATUS, alarm_status));

    constexpr uint16_t SSA_BIT  = (1u << 14); // Status A
    constexpr uint16_t SSBC_BIT = (1u << 15); // Status B

    if (alarm_status & SSA_BIT)
    {
        uint8_t alert_a  = 0;
        uint8_t status_a = 0;
        RETURN_IF_ERR(read_register_byte(SAFETY_A_A, alert_a));
        RETURN_IF_ERR(read_register_byte(SAFETY_S_A, status_a));

        // Alert — momentarily triggered
        if (alert_a & SAFETY_A_SCD)
            LOG_ERROR("ALERT SCD: Short circuit discharge");
        if (alert_a & SAFETY_A_OCD2)
            LOG_ERROR("ALERT OCD2: Overcurrent discharge 2");
        if (alert_a & SAFETY_A_OCD1)
            LOG_ERROR("ALERT OCD1: Overcurrent discharge 1");
        if (alert_a & SAFETY_A_OCC)
            LOG_ERROR("ALERT OCC: Overcurrent charge");
        if (alert_a & SAFETY_A_COV)
            LOG_ERROR("ALERT COV: Cell overvoltage");
        if (alert_a & SAFETY_A_CUV)
            LOG_ERROR("ALERT CUV: Cell undervoltage");

        // Status — still actively faulted
        if (status_a & SAFETY_A_SCD)
            LOG_ERROR("STSTUS SCD: Short circuit discharge active");
        if (status_a & SAFETY_A_OCD2)
            LOG_ERROR("STSTUS OCD2: Overcurrent discharge 2 active");
        if (status_a & SAFETY_A_OCD1)
            LOG_ERROR("STSTUS OCD1: Overcurrent discharge 1 active");
        if (status_a & SAFETY_A_OCC)
            LOG_ERROR("STSTUS OCC: Overcurrent charge active");
        if (status_a & SAFETY_A_COV)
            LOG_ERROR("STSTUS COV: Cell overvoltage active");
        if (status_a & SAFETY_A_CUV)
            LOG_ERROR("STSTUS CUV: Cell undervoltage active");
    }

    if (alarm_status & SSBC_BIT)
    {
        uint8_t alert_b  = 0;
        uint8_t status_b = 0;
        uint8_t alert_c  = 0;
        uint8_t status_c = 0;
        RETURN_IF_ERR(read_register_byte(SAFETY_A_B, alert_b));
        RETURN_IF_ERR(read_register_byte(SAFETY_S_B, status_b));
        RETURN_IF_ERR(read_register_byte(SAFETY_A_C, alert_c));
        RETURN_IF_ERR(read_register_byte(SAFETY_S_C, status_c));

        // Alert B
        if (alert_b & SAFETY_B_OTF)
            LOG_ERROR("ALERT OTF: FET overtemp");
        if (alert_b & SAFETY_B_OTINT)
            LOG_ERROR("ALERT OTINT: Internal die overtemp");
        if (alert_b & SAFETY_B_OTD)
            LOG_ERROR("ALERT OTD: Overtemp discharge");
        if (alert_b & SAFETY_B_OTC)
            LOG_ERROR("ALERT OTC:Overtemp charge");
        if (alert_b & SAFETY_B_UTD)
            LOG_ERROR("ALERT UTD: Undertemp discharge");
        if (alert_b & SAFETY_B_UTC)
            LOG_ERROR("ALERT UTC: Undertemp charge");

        // Status B
        if (status_b & SAFETY_B_OTF)
            LOG_ERROR("STSTUS OTF: FET overtemp active");
        if (status_b & SAFETY_B_OTINT)
            LOG_ERROR("STSTUS OTINT: Internal die overtemp active");
        if (status_b & SAFETY_B_OTD)
            LOG_ERROR("STSTUS OTD: Overtemp discharge active");
        if (status_b & SAFETY_B_OTC)
            LOG_ERROR("STSTUS OTC: Overtemp charge active");
        if (status_b & SAFETY_B_UTD)
            LOG_ERROR("STSTUS UTD: Undertemp discharge active");
        if (status_b & SAFETY_B_UTC)
            LOG_ERROR("STSTUS UTC: Undertemp charge active");

        // Alert C
        if (alert_c & SAFETY_C_OCD3)
            LOG_ERROR("ALERT OCD3: Overcurrent discharge 3");
        if (alert_c & SAFETY_C_SCDL)
            LOG_ERROR("ALERT SCDL: Latched short circuit discharge");
        if (alert_c & SAFETY_C_OCDL)
            LOG_ERROR("ALERT OCDL: Latched overcurrent discharge");
        if (alert_c & SAFETY_C_COVL)
            LOG_ERROR("ALERT COVL: Latched cell overvoltage");
        if (alert_c & SAFETY_C_PTOS)
            LOG_ERROR("ALERT PTOS: Precharge timer suspended");

        // Status C
        if (status_c & SAFETY_C_OCD3)
            LOG_ERROR("STSTUS OCD3: Overcurrent discharge 3 active");
        if (status_c & SAFETY_C_SCDL)
            LOG_ERROR("STSTUS SCDL: Latched short circuit active");
        if (status_c & SAFETY_C_OCDL)
            LOG_ERROR("STSTUS OCDL: Latched overcurrent active");
        if (status_c & SAFETY_C_COVL)
            LOG_ERROR("STSTUS COVL: Latched cell overvoltage active");
        if (status_c & SAFETY_C_PTO)
            LOG_ERROR("STSTUS PTO: Precharge timeout active");
        if (status_c & SAFETY_C_HWDF)
            LOG_ERROR("STSTUS HWDF: Host watchdog fault active");
    }
    return {};
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

} // namespace io::batteryMonitoring

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
