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
    uint8_t checksum = (uint8_t)(sum ^ 0xFF);

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

    uint8_t expected_checksum = (uint8_t)(sum ^ 0xFF);
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
    /* TODO: For now i assumed that deviuce is in full acess mode.
    read Battery Status
    check SEC1/SEC0
    if SEALED:
        send unseal key step 1
        send unseal key step 2
        re-read Battery Status
    if UNSEALED:
        send full access key step 1
        send full access key step 2
        re-read Battery Status
    if FULL ACCESS:
        send SET_CFGUPDATE
    */

    //The device exits SHUTDOWN when it detects a charger, a load detect (our batteries), changing volatge on TS pins? or wake pin. 
    io::time::delay(300);
    RETURN_IF_ERR(hw::i2c::bat_mon.isTargetReady()); // does not CONFIRM the device is out of SHUTDOWN

    // Place the device into CONFIG_UPDATE mode by sending the 0x0090 SET_CFGUPDATE() subcommand. The device will 
    // then automatically disable the protection FETs if they are enabled.

    // Check for deepsleep & wakeup if neccesary
    uint16_t control_status = 0;
    RETURN_IF_ERR(read_register_word(CMD_CONTROL_STATUS, control_status));
    if (control_status & CTRL_STATUS_DEEPSLEEP) //the 2nd bit (DEEPSLEEP)
    {
        RETURN_IF_ERR(write_subcommand(CMD_WAKE_DEEPSLEEP, std::span<const uint8_t>())); //send the SET_CFGUPDATE()
        io::time::delay(5);
    }

    // Check for regular sleep & wakeup if neccesary
    uint16_t battery_status = 0;
    RETURN_IF_ERR(read_register_word(CMD_BATTERY_STATUS, battery_status));
    if (battery_status & BAT_STATUS_SLEEP) //the MSB
    {
        RETURN_IF_ERR(write_subcommand(CMD_WAKE_SLEEP, std::span<const uint8_t>()));
        io::time::delay(300);
    }
    //io::time::delay(10); // Wake up delay

    // CONFIG UPDATE MODE
    // 1. Put device into configuration mode
    RETURN_IF_ERR(write_subcommand(SET_CFGUPDATE, std::span<const uint8_t>()));

    // 2. Wait for the 0x12 Battery Status()[CFGUPDATE] flag to set.
    uint16_t cfgupdate_flag = 0;
    do {

        read_register_word(CMD_BATTERY_STATUS, cfgupdate_flag);
        cfgupdate_flag = cfgupdate_flag & CFGUPDATE_STATUS;
        io::time::delay(1);
    } while ((cfgupdate_flag & CFGUPDATE_STATUS) == 0);

    // 3. Modify settings
    // TODO: Calibration for each cell (TESTING PHASE)

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

    // ALERT
    uint8_t alert = 0x2A; //00101010
    RETURN_IF_ERR(write_data_memory(ALERT, std::span<const uint8_t>(&alert, 1)));

    // VCELL mode
    uint8_t vcell_mode = 0x1B; //00011011
    RETURN_IF_ERR(write_data_memory(VCELL_MODE, std::span<const uint8_t>(&vcell_mode, 1)));

    // Look into the power stuff
    
     // 4. Take device out of configuration mode 
    RETURN_IF_ERR(write_subcommand(EXIT_CFGUPDATE, std::span<const uint8_t>()));

    return {};
}

/**
 * @brief Gets the cell voltage or the entire stack voltage
 * @param uint8_t The subcommand used to read the voltage.
 * @return The battery voltage on success, erorcode if messed up
 */
std::expected<uint16_t, ErrorCode> get_voltage(std::span<uint8_t> voltage_cell)
{
    std::span<uint8_t> voltage_cmd = voltage_cell;
    RETURN_IF_ERR(hw::i2c::bat_mon.transmit(voltage_cmd));
    
    std::span<uint8_t> voltage_buffer;
    RETURN_IF_ERR(hw::i2c::bat_mon.receive(voltage_buffer));

    uint16_t voltage = (uint16_t)(voltage_buffer[0] | (voltage_buffer[1] << 8));
    return voltage;
}

//TODO: OTP configuration
std::expected<void, ErrorCode> OTP(void)
{
    //check if OTP can be written to, the device must be in FULLACCESS and CONFIG_UPDATE mode when this subcommand is sent.

    // CONFIG MODE is only available in FULL ACCESS MODE, which can only be accessed if we transition from SEALED to UNSEALED mode
    // SEALED -> UNSEALED -> FULLACCESS -> FULL CONFIG MODE

    //CRC = x^8 + x^2 + x + 1
    std::span<uint8_t> otp_programming_ok;
    read_subcommand(OTP_WR_CHECK, otp_programming_ok);

    return {};
}

} // namespace io::batteryMonitoring
