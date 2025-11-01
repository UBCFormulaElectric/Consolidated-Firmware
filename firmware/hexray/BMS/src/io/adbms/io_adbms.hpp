/**
 * @file io_adbms.hpp
 * @brief This file contains the interface to the ADBMS driver
 */
#pragma once
#include <stdint.h>
#include "util_errorCodes.h"

namespace io
{
static constexpr uint8_t NUM_SEGMENTS            = 6;
static constexpr uint8_t CELLS_PER_SEGMENTS      = 14;
static constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;
namespace adbms
{
    static constexpr uint8_t REGS_PER_GROUP      = 3;
    static constexpr uint8_t REGISTER_GROUP_SIZE = 6; // 6 bytes

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
        uint8_t gpio_1_8;
        // byte 5
        uint8_t gpio_9_10 : 2;
        uint8_t : 6;
        // byte 6
        uint8_t fc : 3;      // IIR filter parameter, see table 21
        uint8_t comm_bk : 1; // communication break, propagation communication prevention
        uint8_t mute_st : 1; // 1 = mute is active, dischargeing disabled
        uint8_t snap_st : 1; // 1 = snapshot is activated, result registers are frozen
        uint8_t : 2;
    } CFGAR;
    static_assert(sizeof(CFGAR) == adbms::REGISTER_GROUP_SIZE);

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
    void
        io_adbms_readConfigurationRegisters(SegmentConfig configs[io::NUM_SEGMENTS], uint8_t success[io::NUM_SEGMENTS]);

    /**
     * Writes a configuration to all segments on the daisy chain.
     * @param config the configuration to write on the ADBMS
     * @return success of the operation
     */
    ExitCode io_adbms_writeConfigurationRegisters(const SegmentConfig config[io::NUM_SEGMENTS]);

    /**
     * @file adbms/io_adbms_balance.cpp
     */

    /**
     * Sends a command to enable balancing
     * @return success of operation
     */
    ExitCode io_adbms_sendBalanceCommand(void);

    /**
     * Send a command to disable balancing
     * @return success of operation
     */
    ExitCode io_adbms_sendStopBalanceCommand(void);

} // namespace adbms
} // namespace io