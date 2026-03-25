/*
DATASHEET: https://www.zotero.org/groups/5938751/ubc_formula_electric_firmware/collections/E4HTL2J2/items/HK2Z6JZS/reader

*/

#include "hw_i2cs.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"
#include <array>

// I WILL MOVE ALL TS OVER LATER
constexpr uint16_t CMD_CONTROL_STATUS = 0x00; 
constexpr uint16_t CMD_BATTERY_STATUS = 0x12;

// 4 cells 
constexpr uint16_t CMD_CELL1 = 0x14;
//constexpr uint16_t CMD_CELL2 = 0x16;
//constexpr uint16_t CMD_CELL3 = 0x18;
//constexpr uint16_t CMD_CELL4 = 0x1A;

// register addresses 

// command/subcommand addresses 
constexpr uint16_t REG_LOWER     = 0x3E;
constexpr uint16_t REG_UPPER     = 0x3F;
constexpr uint16_t REG_DATA      = 0x40;
constexpr uint16_t REG_CHECKSUM  = 0x60; 
constexpr uint16_t REG_DATALENGTH = 0x61;

constexpr uint16_t SUBCOMMAND_BYTES = 4; // account for the 0x3E/0x3F and 0x61 and 0x60

// regsiter bitmasks 
constexpr uint16_t BAT_STAT_SLEEP      = (1 << 15);
constexpr uint16_t CTRL_STAT_DEEPSLEEP = (1 << 14);
// wake commands
constexpr uint16_t CMD_WAKE_SLEEP        = 0x009A;
constexpr uint16_t CMD_WAKE_DEEPSLEEP    = 0x000E;
namespace io::batteryMonitoring 
{

// TODO: use this helper fucntion for every time i repeat ts
std::expected<uint16_t, ErrorCode> read_register(uint16_t cmd)
{
    std::array<uint8_t, 2> buf;
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(cmd, buf));
    
    uint16_t result = (uint16_t)((buf[1] << 8) | buf[0]);
    
    return result;
}
// should contain a 0 for cell_voltage[2] as per the interconnect [x, x, 0, x, x]
uint16_t cell_voltages[5]; 

std::expected<void, ErrorCode> read_cell_voltage(void)
{
    for (uint16_t i = 0; i < 5; i++) 
    {
        std::array<uint8_t, 2> buf;
        uint16_t cmd = (uint16_t)(CMD_CELL1 + (i * 2u)); 
        RETURN_IF_ERR_SILENT(hw::i2c::bat_mon.memoryRead(cmd, buf));
        cell_voltages[i] = (uint16_t)((buf[1] << 8) | buf[0]);
    }
    return {};
}

uint16_t calculate_pack_voltage(void)
{
    uint16_t pack_voltage = 0;
    for (int i = 0; i < 5; i++)
    {
        pack_voltage += cell_voltages[i];
    }
    return pack_voltage;
}

// splits subcommand and writes to 0x3E and 0x3F, writes data at 0x40, then checksum/length at 0x60/0x61.
std::expected<void, ErrorCode> write_command_buffer(uint16_t sub_cmd, std::span<const uint8_t> data)
{
    // must be 32 at max
    if (data.size() > 32)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = (uint8_t)(sub_cmd >> 8);
    uint8_t low_byte = (uint8_t)(sub_cmd & 0x00FF);

    // total length: subcommand bytes + transfer-buffer bytes + checksum/length bytes.
    uint8_t data_transfer = SUBCOMMAND_BYTES + (uint8_t)(data.size());

    // checksum = inverted 8-bit sum of subcommand bytes and transfer-buffer data bytes.
    uint16_t sum = low_byte + high_byte;
    for (uint8_t byte : data)
    {
        sum += byte;
    }
    uint8_t checksum = (uint8_t)(sum ^ 0xFF);

    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_LOWER, std::span<const uint8_t>(&low_byte, 1)));
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_UPPER, std::span<const uint8_t>(&high_byte, 1)));

    // action only subcommands require only 0x3E/0x3F writes.
    if (data.empty())
    {
        return {};
    }

    // 0x60 = checksum, 0x61 = length and write together.
    std::array<uint8_t, 2> checksum_and_length = {{checksum, data_transfer}};
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_DATA, data));
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_CHECKSUM, checksum_and_length));

    return {};
}

// reads subcommand response using 0x3E/0x3F completion poll, then 0x61 length and 0x40 buffer.
std::expected<void, ErrorCode> read_command_buffer(uint16_t sub_cmd, std::span<uint8_t> data)
{
    if (data.size() > 32)
    {
        return std::unexpected(ErrorCode::OUT_OF_RANGE);
    }

    uint8_t high_byte = (uint8_t)(sub_cmd >> 8);
    uint8_t low_byte = (uint8_t)(sub_cmd & 0x00FF);

    // 1. Write lower byte of subcommand to 0x3E
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_LOWER, std::span<const uint8_t>(&low_byte, 1)));
    // 2. Write upper byte of subcommand to 0x3F
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryWrite(REG_UPPER, std::span<const uint8_t>(&high_byte, 1)));

    /* 3. Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation  yet. When the subcommand has 
    completed, the readback will return what was originally written. Continue reading 0x3E and 0x3F until it returns what was written originally */
    uint8_t low_status = 0xFF;
    uint8_t high_status = 0xFF;
    while ((low_status != low_byte) || (high_status != high_byte))
    {
        RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_LOWER, std::span<uint8_t>(&low_status, 1)));
        RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_UPPER, std::span<uint8_t>(&high_status, 1)));
        io::time::delay(1);
    }
    //4. Read the length of response from 0x61
    uint8_t total_len = 0;
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_DATALENGTH, std::span<uint8_t>(&total_len, 1)));

    //5. Read buffer starting at 0x40 for the expected length
    uint8_t buffer_len = (uint8_t)(total_len - SUBCOMMAND_BYTES);
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_DATA, data.first(buffer_len)));

    // 6. Read the checksum at 0x60 and verify it matches the data read.
    uint8_t received_checksum = 0;
    RETURN_IF_ERR(hw::i2c::bat_mon.memoryRead(REG_CHECKSUM, std::span<uint8_t>(&received_checksum, 1)));

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

std::expected<void, ErrorCode> init(void)
{
    io::time::delay(10);
    RETURN_IF_ERR(hw::i2c::bat_mon.isTargetReady());

    std::array<uint8_t, 2> stat_buf;
    
    // check in 0x12 if its in regular sleep mode 
    RETURN_IF_ERR_SILENT(hw::i2c::bat_mon.memoryRead(CMD_BATTERY_STATUS, stat_buf));
    uint16_t battery_status = (uint16_t)((stat_buf[1] << 8) | stat_buf[0]);
    if (battery_status & BAT_STAT_SLEEP) {
        RETURN_IF_ERR(write_command_buffer(CMD_WAKE_SLEEP, std::span<const uint8_t>())); /* doesnt send data and refer to the exit 
        statement in func that handles ts, send size 0*/
        io::time::delay(5);
    }
    
    // check in 0x00 if in deep sleep 
    RETURN_IF_ERR_SILENT(hw::i2c::bat_mon.memoryRead(CMD_CONTROL_STATUS, stat_buf));
    uint16_t control_status = (uint16_t)((stat_buf[1] << 8) | stat_buf[0]);
    if (control_status & CTRL_STAT_DEEPSLEEP) {
        RETURN_IF_ERR(write_command_buffer(CMD_WAKE_DEEPSLEEP, std::span<const uint8_t>()));
        io::time::delay(5);
    }

    //keep in mind for the vcell mode register i need to write 0x001B due to the config we have rn for how the cells taps are bridged.

    return {};
}
} // namespace io::batteryMonitoring
