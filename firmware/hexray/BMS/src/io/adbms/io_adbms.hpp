/**
 * @file io_adbms.hpp
 * @brief This file contains the interface to the ADBMS driver
 */
#pragma once
#include <cstdint>
#include "util_errorCodes.h"

namespace io
{
static constexpr uint8_t NUM_SEGMENTS            = 6;
static constexpr uint8_t CELLS_PER_SEGMENTS      = 14;
static constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;
namespace adbms
{
    static constexpr uint8_t REGS_PER_GROUP          = 3; // 3 x 16-bit = 6 bytes
    static constexpr uint8_t REGISTER_GROUP_SIZE     = 6; // bytes
    static constexpr uint8_t VOLTAGE_REGISTER_GROUPS = 5; // A..E only (F not used; 14 cells measured)

    /**
     * @file adbms/io_adbms_configs.cpp
     */

    // as per table 55
    typedef struct
    {
        // byte 1
        uint8_t cth : 3;    // C-ADC vs. S-ADC comparison voltage threshold
        uint8_t : 4;        // unused bits
        uint8_t ref_on : 1; // reference powered up, enables REFUP state
        // byte 2
        uint8_t flag_d; // asserts flags in Status Reg C for latent fault detection (pg. 70)
        // byte 3
        uint8_t : 3;
        uint8_t owa : 3;     // open wire soak times
        uint8_t owrng : 1;   // soak time range
        uint8_t soak_on : 1; // enables soak on AUX ADCs
        // byte 4
        uint8_t gpio_1_8; // GPIOx pin control
        // byte 5
        uint8_t gpio_9_10 : 2; // GPIOx pin control
        uint8_t : 6;           // unused bits
        // byte 6
        uint8_t fc : 3;      // IIR filter parameter, see table 21
        uint8_t comm_bk : 1; // communication break, propagation communication prevention
        uint8_t mute_st : 1; // 1 = mute is active, dischargeing disabled
        uint8_t snap_st : 1; // 1 = snapshot is activated, result registers are frozen
        uint8_t : 2;
    } CFGA;
    static_assert(sizeof(CFGA) == adbms::REGISTER_GROUP_SIZE);

    // as per table 56
    typedef struct
    {
        // byte 1
        uint8_t vuv_0_7; // cell undervoltage threshold = vuv * 16 * 150uV + 1.5V
        // byte 2
        uint8_t vuv_8_11 : 4;
        uint8_t vov_0_3 : 4; // cell overvoltage threshold = vov * 16 * 150uV + 1.5V
        // byte 3
        uint8_t vov_4_11;
        // byte 4
        uint8_t dcto_0_5 : 6; // dicharge timeout value
        uint8_t dtrng : 1;    // discharge timer range setting
        uint8_t dtmen : 1;    // enable discharge timer monitor
        // byte 5
        uint8_t dcc_1_8; // discharge cell x
        // byte 6
        uint8_t dcc_9_16;
    } CFGBR;
    static_assert(sizeof(CFGBR) == adbms::REGISTER_GROUP_SIZE);

    typedef struct
    {
        CFGAR reg_a;
        CFGBR reg_b;
    } SegmentConfig;

    /**
     * Reads the configuration registers, and returns them into the pointer you give it
     * @param configs pointer to array of SegmentConfig structs to populate
     * @param success success of the operation
     */
    void readConfigurationRegisters(SegmentConfig configs[io::NUM_SEGMENTS], uint8_t success[io::NUM_SEGMENTS]);

    /**
     * Writes a configuration to all segments on the daisy chain.
     * @param config the configuration to write on the ADBMS
     * @return success of the operation
     */
    ExitCode writeConfigurationRegisters(const SegmentConfig config[io::NUM_SEGMENTS]);

    /**
     * @file adbms/io_adbms_balance.cpp
     */

    /**
     * Sends a command to enable balancing
     * @return success of operation
     */
    ExitCode sendBalanceCommand(void);

    /**
     * Send a command to disable balancing
     * @return success of operation
     */
    ExitCode sendStopBalanceCommand(void);

    /**
     * @file io_adbms_cells.cpp
     */

    /**
     * Gets the value of the cell voltage registers, shares success of operation
     * @param cell_voltage_regs value of the cell voltage registers
     * @param comm_success success of operation on a certain segment and register group
     */
    void readVoltageRegisters(
        uint16_t cell_voltage_regs[io::NUM_SEGMENTS][io::CELLS_PER_SEGMENTS],
        ExitCode comm_success[io::NUM_SEGMENTS][io::CELLS_PER_SEGMENTS]);

    /**
     * sends a command to read all voltages from all segments
     * @return success of the operation
     */
    ExitCode startCellsAdcConversion(void);

    /**
     * @file ltc6813/io_ltc6813_utils.c
     */

    /**
     * Wake up LTCs. Sends a dummy command (RDCFGA). See "Waking Up the Serial Interface" in the datasheet.
     */
    void wakeup(void);

    /**
     * polls the LTC6813 for the completion of the ADC conversions
     * @return success of the operation
     */
    ExitCode pollAdcConversions(void);

} // namespace adbms
} // namespace io