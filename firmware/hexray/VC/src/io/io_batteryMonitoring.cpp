/*
DATASHEET: https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader
TODO: could perchance add a fucntion that differs from subcommands vs direct commands...
*/

#include "hw_i2cs.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"

namespace io::batteryMonitoring 
{

static std::expected<void, ErrorCode> write_data_memory(uint16_t addr, std::span<const uint8_t> data);

static std::expected<void, ErrorCode> write_register_byte(uint16_t reg, uint8_t value)
{
    return hw::i2c::bat_mon.memoryWrite(reg, std::span<const uint8_t>(&value, 1));
}

static std::expected<void, ErrorCode> read_register_byte(uint16_t reg, uint8_t &value)
{
    return hw::i2c::bat_mon.memoryRead(reg, std::span<uint8_t>(&value, 1));
}

static std::expected<void, ErrorCode> read_register_word(uint16_t reg, uint16_t &value)
{
    std::array<uint8_t, 2> buf;
    RETURN_IF_ERR_SILENT(hw::i2c::bat_mon.memoryRead(reg, buf));
    value = (uint16_t)((buf[1] << 8) | buf[0]);
    return {};
}

static std::expected<void, ErrorCode> protectionInit(void)
{
    uint8_t autonomous_protection = 0x10; //00010000
    RETURN_IF_ERR(write_data_memory(MFG_STATUS_INIT, std::span<const uint8_t>(&autonomous_protection, 1)));

    return {};
}

// Writes to a command code, which tells the chip to do something
static std::expected<void, ErrorCode> write_transfer(uint16_t sub_cmd, std::span<const uint8_t> data)
{
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = (uint8_t)(sub_cmd >> 8);
    uint8_t low_byte = (uint8_t)(sub_cmd & 0x00FF);

    // total length: subcommand bytes + transfer-buffer bytes + checksum/length bytes.
    uint8_t length = static_cast<uint8_t>(data.size() + 4u);

    // checksum = inverted 8-bit sum of subcommand bytes and transfer-buffer data bytes.
    uint16_t sum = low_byte + high_byte;
    for (uint8_t byte : data)
    {
        sum += byte;
    }
    uint8_t checksum = static_cast<uint8_t>(0xFFu - (sum & 0x00FFu));;

    RETURN_IF_ERR(write_register_byte(REG_LOWER, low_byte));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, high_byte));

    // action only subcommands require only 0x3E/0x3F writes.
    if (data.empty())
    {
        return {};
    }

    // 0x60 = checksum, 0x61 = length and write together.
    std::array<uint8_t, 2> checksum_and_length = {{checksum, length}};
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_DATA, data));
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_CHECKSUM, checksum_and_length));

    return {};
}

// Tells the chip to do something and reads response 
[[maybe_unused]] static std::expected<void, ErrorCode> read_subcommand(uint16_t sub_cmd, std::span<uint8_t> data)
{
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = (uint8_t)(sub_cmd >> 8);
    uint8_t low_byte = (uint8_t)(sub_cmd & 0x00FF);

    // 1. Write lower byte of subcommand to 0x3E
    RETURN_IF_ERR(write_register_byte(REG_LOWER, low_byte));
    // 2. Write upper byte of subcommand to 0x3F
    RETURN_IF_ERR(write_register_byte(REG_UPPER, high_byte));

    /* 3. Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation  yet. When the subcommand has 
    completed, the readback will return what was originally written. Continue reading 0x3E and 0x3F until it returns what was written originally */
    uint8_t low_status = 0xFF;
    uint8_t high_status = 0xFF;
    while ((low_status != low_byte) || (high_status != high_byte))
    {
        RETURN_IF_ERR(read_register_byte(REG_LOWER, low_status));
        RETURN_IF_ERR(read_register_byte(REG_UPPER, high_status));
        io::time::delay(1);
    }

    // 4. Read the length of response from 0x61
    uint8_t total_len = 0;
    RETURN_IF_ERR(read_register_byte(REG_DATALENGTH, total_len));
    if (total_len < SUBCOMMAND_BYTES)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    // 5. Read buffer starting at 0x40 for the expected length
    uint8_t buffer_len = (uint8_t)(total_len - SUBCOMMAND_BYTES);
    if (buffer_len > data.size())
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_DATA, data.first(buffer_len)));

    // 6. Read the checksum at 0x60 and verify it matches the data read.
    uint8_t received_checksum = 0;
    RETURN_IF_ERR(read_register_byte(REG_CHECKSUM, received_checksum));

    uint16_t sum = low_byte + high_byte;
    for (uint8_t byte : data.first(buffer_len))
    {
        sum += byte;
    }

    uint8_t expected_checksum = static_cast<uint8_t>(0xFFu - (sum & 0x00FFu));
    if (received_checksum != expected_checksum)
    {
        return std::unexpected(ErrorCode::CHECKSUM_FAIL);
    }

    return {};
}

static std::expected<void, ErrorCode> read_data_memory(uint16_t addr, std::span<uint8_t> data)
{
    if (data.size() > 32u)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = static_cast<uint8_t>(addr >> 8);
    uint8_t low_byte  = static_cast<uint8_t>(addr & 0x00FF);

    RETURN_IF_ERR(write_register_byte(REG_LOWER, low_byte));
    RETURN_IF_ERR(write_register_byte(REG_UPPER, high_byte));

    uint8_t total_len = 0;
    RETURN_IF_ERR(read_register_byte(REG_DATALENGTH, total_len));
    if (total_len < SUBCOMMAND_BYTES)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t buffer_len = static_cast<uint8_t>(total_len - SUBCOMMAND_BYTES);
    if (buffer_len > data.size())
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_DATA, data.first(buffer_len)));

    uint8_t received_checksum = 0;
    RETURN_IF_ERR(read_register_byte(REG_CHECKSUM, received_checksum));

    uint16_t sum = low_byte + high_byte;
    for (uint8_t byte : data.first(buffer_len))
    {
        sum += byte;
    }

    uint8_t expected_checksum = static_cast<uint8_t>(0xFFu - (sum & 0xFFu));
    if (received_checksum != expected_checksum)
    {
        return std::unexpected(ErrorCode::CHECKSUM_FAIL);
    }

    return {};
}

// Wrappers for simplicity
static std::expected<void, ErrorCode> write_subcommand(uint16_t sub_cmd, std::span<const uint8_t> data)
{
    return write_transfer(sub_cmd, data);
}

static std::expected<void, ErrorCode> write_data_memory(uint16_t addr, std::span<const uint8_t> data)
{
    return write_transfer(addr, data);
}

std::expected<void, ErrorCode> init(void)
{
    //The device exits SHUTDOWN when it detects a charger, a load detect (our batteries), changing volatge on TS pins? or wake pin. 
    io::time::delay(300);
    RETURN_IF_ERR(hw::i2c::bat_mon.isTargetReady()); // does not CONFIRM the device is out of SHUTDOWN

   // Check for deepsleep & wakeup if neccesary
    uint16_t control_status = 0;
    RETURN_IF_ERR(read_register_word(CMD_CONTROL_STATUS, control_status));
    if (control_status & CTRL_STATUS_DEEPSLEEP) //the 2nd bit (DEEPSLEEP)
    {
        LOG_INFO("Battery is currently in deepsleep mode");
        RETURN_IF_ERR(write_subcommand(CMD_WAKE_DEEPSLEEP, {})); //send the SET_CFGUPDATE()
        io::time::delay(300);
    }

    // Check for regular sleep & wakeup if neccesary
    uint16_t battery_status = 0;
    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, battery_status));
    if (battery_status & BAT_STATUS_SLEEP) //the MSB
    {
        LOG_INFO("Battery is currently in sleep mode");
        RETURN_IF_ERR(write_subcommand(CMD_WAKE_SLEEP, {}));
        io::time::delay(300);
    }
    //io::time::delay(10); // Wake up delay

    /*SECURITY MODES*/
    uint16_t security_state;
    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state));
    security_state = ((security_state & 0x0300) >> 8);
 
    /* uint16_t full_access = 0x1100; //idk anything but 0xFFFF
    std::array<uint8_t, 2> full_access_bytes = {{static_cast<uint8_t>(full_access & 0x00FF), 
    static_cast<uint8_t>((full_access >> 8) & 0x00FF)}};

    RETURN_IF_ERR(write_data_memory(FULL_ACCESS_EDIT, full_access_bytes)); */
 
    if (security_state == 0x3) 
    {
        RETURN_IF_ERR(write_subcommand(SECURITY_UNSEAL_FIRST, {}));
        RETURN_IF_ERR(write_subcommand(SECURITY_UNSEAL_SECOND, {}));
    }
 
    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state));
    security_state = ((security_state & 0x0300) >> 8);
 
    if (security_state == 0x2) 
    {
        RETURN_IF_ERR(write_subcommand(SECURITY_FULLACESS, {}));
        RETURN_IF_ERR(write_subcommand(SECURITY_FULLACESS, {})); // look into ts
        //RETURN_IF_ERR(write_subcommand(full_access, {}));
    }
 
    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, security_state));
    security_state = ((security_state & 0x0300) >> 8);

    /* Place the device into CONFIG_UPDATE mode by sending the 0x0090 SET_CFGUPDATE() subcommand. The device will 
    then automatically disable the protection FETs if they are enabled. */

    /*CONFIG UPDATE MODE*/
    // 1. Put device into configuration mode
    if (security_state == 0x1) 
    {
        RETURN_IF_ERR(write_subcommand(SET_CFGUPDATE, {}));
    }

    // 2. Wait for the 0x12 Battery Status()[CFGUPDATE] flag to set.
    uint16_t cfgupdate_flag = 0;
    do {
        RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, cfgupdate_flag));
        cfgupdate_flag = cfgupdate_flag & CFGUPDATE_STATUS;
        io::time::delay(1);
    } while ((cfgupdate_flag & CFGUPDATE_STATUS) == 0);

    // 3. Modify settings
    // TODO: 
    /* CALIBRATION (TESTING PHASE) */

    /* CONFIGURATION */
    // REG0 needs to feed REG1, confugure first because we don't have external supply for REGIN 5V
    uint8_t reg0;
    RETURN_IF_ERR(read_data_memory(REG0_CONFIG, std::span<uint8_t>(&reg0, 1))); //0000000_(RSVD)_(EN)
    reg0 |= 0x1;
    RETURN_IF_ERR(write_data_memory(REG0_CONFIG, std::span<const uint8_t>(&reg0, 1)));

    // Modify REG1 to be 3V3
    uint8_t reg1 = 0x0D; //00001101 
   RETURN_IF_ERR(write_data_memory(REG1_CONFIG, std::span<const uint8_t>(&reg1, 1)));

    // I2C Address
    uint8_t i2c_addy = 0x10;
    RETURN_IF_ERR(write_data_memory(I2C_ADDY, std::span<const uint8_t>(&i2c_addy, 1)));

    // ALERT: rmr to interpret this as nALERT on the MCU
    uint8_t alert = 0x2A; //00101010
    RETURN_IF_ERR(write_data_memory(ALERT, std::span<const uint8_t>(&alert, 1)));

    // VCELL mode
    uint8_t vcell_mode = 0x1B; //00011011
    RETURN_IF_ERR(write_data_memory(VCELL_MODE, std::span<const uint8_t>(&vcell_mode, 1)));

    /* PROTECTION */
    RETURN_IF_ERR(protectionInit());
    
     // 4. Take device out of configuration mode 
    RETURN_IF_ERR(write_subcommand(EXIT_CFGUPDATE, {}));

    return {};
}

/**
 * @brief Gets the cell voltage or the entire stack voltage
 * @param CellNum The command used to read the voltage
 * @return The voltage on success, erorcode if messed up
 */
std::expected<uint16_t, ErrorCode> get_voltage(CellNum cell)
{
    uint16_t voltage_cmd = 0;

    switch (cell)
    {
        case CELL1:
            voltage_cmd = CELL1_MV;
            break;
        case CELL2:
            voltage_cmd = CELL2_MV;
            break;
        case CELL3:
            voltage_cmd = CELL3_MV; // Should read 0
            break;
        case CELL4:
            voltage_cmd = CELL4_MV;
            break;
        case CELL5:
            voltage_cmd = CELL5_MV;
            break;
        default:
            return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint16_t voltage = 0;
    RETURN_IF_ERR(read_register_word(voltage_cmd, voltage));
    return voltage;
}

/**
 * @brief Gets the raw ADCs for the current and volatge
 * @param CellNum The subcommand
 * @return The ADC voltage/current on success, erorcode if messed up
 */
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

    uint32_t reading = static_cast<uint32_t>(data[value_start])
                     | (static_cast<uint32_t>(data[value_start + 1]) << 8)
                     | (static_cast<uint32_t>(data[value_start + 2]) << 16)
                     | (static_cast<uint32_t>(data[value_start + 3]) << 24);

    return reading;
}
//TODO: OTP configuration
std::expected<void, ErrorCode> OTP(void)
{
    //check if OTP can be written to, the device must be in FULLACCESS and CONFIG_UPDATE mode when this subcommand is sent.

    // CONFIG MODE is only available in FULL ACCESS MODE, which can only be accessed if we transition from SEALED to UNSEALED mode
    // SEALED -> UNSEALED -> FULLACCESS -> FULL CONFIG MODE

    //CRC = x^8 + x^2 + x + 1
    // std::span<uint8_t> otp_programming_ok;
    // read_subcommand(OTP_WR_CHECK, otp_programming_ok);

    return {};
}

} // namespace io::batteryMonitoring
