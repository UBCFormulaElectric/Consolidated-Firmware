/*
DATASHEET:
https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
*/

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"

namespace io::batteryMonitoring
{

// Static function declarations
static std::expected<void, ErrorCode> read_register(uint16_t reg, std::span<uint8_t> data);
static std::expected<void, ErrorCode> write_register(uint16_t reg, std::span<uint8_t> data);

static std::expected<void, ErrorCode> command_read_byte(uint8_t command_addr, uint8_t* data);
static std::expected<void, ErrorCode> command_write_byte(uint8_t command_addr, uint8_t data);
static std::expected<void, ErrorCode> command_read_2byte(uint8_t command_addr, uint16_t* data);
static std::expected<void, ErrorCode> command_write_2byte(uint8_t command_addr, uint16_t data);

static std::expected<void, ErrorCode> read_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static std::expected<void, ErrorCode> write_subcommand(uint16_t sub_cmd, std::span<uint8_t> data);
static std::expected<void, ErrorCode> execute_subcommand(uint16_t sub_cmd);

static std::expected<void, ErrorCode> fetsInit(void);

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
[[maybe_unused]] static std::expected<void, ErrorCode> command_read_byte(uint8_t command_addr, uint8_t* data)
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
static std::expected<void, ErrorCode> command_read_2byte(uint8_t command_addr, uint16_t* data)
{
    uint8_t rx_buffer[2] = {0, 0};
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
    RETURN_IF_ERR(write_register(REG_UPPER,std::span<uint8_t>(&high_byte, 1)));

    /* 3. Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation  yet. When
    the subcommand has completed, the readback will return what was originally written. Continue reading 0x3E and 0x3F
    until it returns what was written originally */
    uint8_t low_status   = 0xFF;
    uint8_t high_status  = 0xFF;
    bool subcmd_ready = false;
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

    // 5. Combine Checksum and Length into a single 16-bit word, checksum goes to 0x60 (lower byte), length goes to 0x61 (upper byte)
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

/* -------------------- Voltage and Current Readings ------------------------- */
/**
 * @brief Gets the cell voltage
 * @param CellReading The command used to read the voltage
 * @return The voltage on success, erorcode if messed up
 */
std::expected<uint16_t, ErrorCode> get_voltage_cell(CellReading cell)
{
    uint16_t cell_voltage = 0;
    RETURN_IF_ERR(command_read_2byte(cell, &cell_voltage));
    return static_cast<uint16_t>(cell_voltage);
}
/**
 * @brief Gets the system voltage
 * @param SystemReading The command used to read the voltage
 * @return The voltage on success, erorcode if messed up
 */
std::expected<uint16_t, ErrorCode> get_voltage_system(SystemReading system)
{
    uint16_t cv_voltage = 0;
    RETURN_IF_ERR(command_read_2byte(system, &cv_voltage));
    return static_cast<uint16_t>(cv_voltage * 10); //  The units for TOS, PACK, and LD voltages are reported in cV (10mV LSB) by default
}

[[maybe_unused]] static std::expected<void, ErrorCode> protectionInit(void)
{
    /*uint8_t manual_protection = 0x10; // 00010000
    RETURN_IF_ERR(write_data_memory(MFG_STATUS_INIT, std::span<const uint8_t>(&autonomous_protection, 1))); */

    uint8_t sleep_chg = 0x02;
    RETURN_IF_ERR(write_subcommand(FET_FET_OPTION, std::span<uint8_t>(&sleep_chg, 1)));

    return {};
}

static std::expected<void, ErrorCode> fetsInit(void)
{
    RETURN_IF_ERR(execute_subcommand(SUBCMD_ALL_FETS_ON)); // 0x0096
    LOG_INFO("FETs enabled");
    return {};
}

void random(void)
{
    
}

std::expected<void, ErrorCode> init(void)
{
    // 1. Is chip responsive 
    // RETURN_IF_ERR(bat_mon.isTargetReady());

    // 2.0 Check to see if chip is in DEEPSLEEP
    uint16_t control_status = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_CONTROL_STATUS, &control_status));
    while (control_status & CTRL_STATUS_DEEPSLEEP)
    {
        LOG_INFO("Device in deepsleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_DEEPSLEEP));
    }
    LOG_INFO("Device is out of deepsleep mode");

    // 2.1 Check to see if the device is in SLEEP
    uint16_t battery_status = 0;
    RETURN_IF_ERR(command_read_2byte(CMD_BATTERY_STATUS, &battery_status));
    while (battery_status & BAT_STATUS_SLEEP)
    {
        LOG_INFO("Device in sleep mode");
        RETURN_IF_ERR(execute_subcommand(SUBCMD_WAKE_SLEEP));
        LOG_INFO("Device is out of sleep mode");
    }
    
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
        io::time::delay(1);
    }
    if (!cfgupdate_ready)
    {
        LOG_ERROR("Failed to enter CONFIG_UPDATE");
        return std::unexpected(ErrorCode::TIMEOUT);
    }

    // 4. Modify settings

    // ALERT
    uint8_t alert = 0x02; // Could potentially also be 0x2A (for now I assumed high impedance)
    RETURN_IF_ERR(write_subcommand(ALERT, std::span<uint8_t>(&alert, 1)));

    // VCELL mode
    uint8_t vcell_mode = 0x1B;
    RETURN_IF_ERR(write_subcommand(VCELL_MODE, std::span<uint8_t>(&vcell_mode, 1)));

    // PROTECTION
    // RETURN_IF_ERR(protectionInit());

    // Add one that edits the security registers (OTP SHIT)

    // 4. Take device out of configuration mode
    RETURN_IF_ERR(execute_subcommand(EXIT_CFGUPDATE));

    fetsInit();

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
    