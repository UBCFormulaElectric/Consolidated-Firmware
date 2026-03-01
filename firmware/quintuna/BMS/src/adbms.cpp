#include "adbms.h"
#include "io_adbms.hpp"
#include "app_segments.hpp"
#include "util_errorCodes.hpp"
#include "io_time.hpp"

#include <cassert>
#include <array>

static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configs;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS> success;
static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configReg;

static std::array<std::array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                        cell_voltage_regs;
static std::array<std::array<std::expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>  comm_success;

CFUNC void adbms_init()
{
    for (auto &[reg_a, reg_b] : configReg)
    {
        reg_a.cth       = 1;
        reg_a.ref_on    = 1;
        reg_a.flag_d    = 0;
        reg_a.owa       = 0;
        reg_a.owrng     = 0;
        reg_a.soak_on   = 0;
        reg_a.gpio_1_8  = 0b11111111;
        reg_a.gpio_9_10 = 0b11;
        reg_a.fc        = 0;
        reg_a.comm_bk   = 0;
        reg_a.mute_st   = 0;
        reg_a.snap_st   = 0;

        constexpr uint16_t vuv = 0b110100001;
        constexpr uint16_t vov = 0b10001100101;
        reg_b.vuv_0_7          = static_cast<uint8_t>(vuv & 0xFF);
        reg_b.vuv_8_11         = static_cast<uint8_t>(vuv >> 8 & 0x0F);
        reg_b.vov_0_3          = static_cast<uint8_t>(vov & 0x0F);
        reg_b.vov_4_11         = static_cast<uint8_t>(vov >> 4 & 0xFF);
        reg_b.dcto_0_5         = 0;
        reg_b.dtrng            = 0;
        reg_b.dtmen            = 0;
        reg_b.dcc_1_8          = 0xFF;
        reg_b.dcc_9_16         = 0xFF;
    }

    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigReg(configReg));
}

// ------- start Jack's DMA code -------

// Buffers for SPI DMA transfers. Placed in SRAM4 for optimal access by the SPI peripheral and DMA controller.
__attribute__((section(".sram4"))) uint8_t bms_tx_buf[84];
__attribute__((section(".sram4"))) uint8_t bms_rx_buf[84];

volatile bool bms_transfer_complete = false; // volatile since it's set in an ISR

// Prepare command and start SPI DMA
void start_adbms_read() {
    bms_transfer_complete = false;

    // Prepare tx_buf
    bms_tx_buf[0] = 0x00; // Example
    bms_tx_buf[1] = 0x04; // Example
    // TODO: Insert PEC15 calculation for the command here
    bms_tx_buf[2] = 0x07; // Example
    bms_tx_buf[3] = 0xC2; // Example
    
    for(int i = 4; i < 84; i++) bms_tx_buf[i] = 0xFF; // Dummy bytes for clocking out data

    // DMA
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive_DMA(&hspi4, bms_tx_buf, bms_rx_buf, 84);
    
    if (status != HAL_OK) {
        // Handle error (SPI busy or config issue)
    }
}

// ISR callback for SPI DMA TC (should prob go in a different file that has all the ISRs)
extern "C" {
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI4) {

        bms_transfer_complete = true; // Signal the main loop
    }
}
}

// Put in main loop for running PEC10 checks
void process_bms_data() {
    if (bms_transfer_complete) {
        // Data starts at index 4 because of the 4-byte command garbage shloppy
        for (int chip = 0; chip < 10; chip++) {
            uint8_t* segment = &bms_rx_buf[4 + (chip * 8)];
            
            // segment[0-5] = Cell Voltages
            // segment[6]   = Command Counter
            // segment[7]   = PEC10 (Verify this!)
            
            if (verify_pec10(segment, 7, segment[7])) {
                // Successful CRC: Convert bytes to floats
                float voltage = ((segment[1] << 8) | segment[0]) * 0.000150f; 
            } else {
                // Else: Noise on the line or some bs
            }
        }
        bms_transfer_complete = false; // Reset for next cycle
    }
}

// ------- end Jack's DMA code -------

CFUNC void adbms_tick()
{
    LOG_INFO("tick!");
    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigReg(configReg));
    io::adbms::readConfigReg(configs, success);
    //io::adbms::startCellsAdcConversion();
    //io::time::delay(2);
    //io::adbms::readCellVoltageReg(cell_voltage_regs, comm_success);
    //assert(io::adbms::sendBalanceCmd());
    LOG_INFO("done!");
}

