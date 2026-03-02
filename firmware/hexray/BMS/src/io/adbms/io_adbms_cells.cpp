#include "io_adbms_internal.hpp"
#include <cstdint>

static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS>      group_success;
//static std::array<std::array<uint8_t, io::adbms::REG_GROUP_SIZE>, io::NUM_SEGMENTS> regs;

namespace io::adbms
{
constexpr uint8_t MAX_NUM_ATTEMPTS  = 10U;
constexpr uint8_t ADCV_STATUS_READY = 0x3F;
//constexpr uint8_t CELLS_PER_GROUP   = 3U;

std::expected<void, ErrorCode> clearCellVoltageReg()
{
    return sendCmd(CLRCELL);
}

std::expected<void, ErrorCode> startCellsAdcConversion()
{
    RETURN_IF_ERR(clearCellVoltageReg());
    return sendCmd(ADCV_BASE);
}

std::expected<void, ErrorCode> pollCellsAdcConversion()
{
    for (uint8_t attempt = 0U; attempt < MAX_NUM_ATTEMPTS; attempt++)
    {
        uint8_t rx_data;
        RETURN_IF_ERR(poll(PLCADC, { &rx_data, sizeof(rx_data) }));
        if (rx_data == ADCV_STATUS_READY)
        {
            return {};
        }
    }
    return std::unexpected(ErrorCode::TIMEOUT);
}


// void readCellVoltageReg(
//     std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &cell_voltage_regs,                 
//     std::array<std::array<std::expected<void, ErrorCode>, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &comm_success
// )
// {
//     const std::expected<void, ErrorCode> poll_ok = pollCellsAdcConversion();

//     if (!poll_ok){
//         for (uint8_t i = 0U; i < NUM_SEGMENTS; i++){
//             for (uint8_t j = 0U; j < CELLS_PER_SEGMENT; j++){
//                 comm_success[i][j] = poll_ok;
//             }
//         }
//         return;
//     }

    
//     readRegGroup(RDCVALL, regs, group_success);

//     cell_voltage_regs = static_cast<uint16_t> regs;

//     for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++){
//         for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++){
//             comm_success[seg][cell] = group_success[seg];
//         }
//     }
    
// }
} // namespace io::adbms
