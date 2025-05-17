#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "hw_i2cs.h"

/**
 * PAC195X sampling modes (SAMPLE_MODE[3:0] in CTRL reg, bits 15–12)
 * :contentReference[oaicite:6]{index=6}:contentReference[oaicite:7]{index=7}
 */
typedef enum
{
    PAC195X_MODE_1024_ACCU = 0x0, ///< 1024 SPS adaptive accumulation
    PAC195X_MODE_256_ACCU  = 0x1, ///< 256 SPS adaptive accumulation
    PAC195X_MODE_64_ACCU   = 0x2, ///< 64 SPS adaptive accumulation
    PAC195X_MODE_8_ACCU    = 0x3, ///< 8 SPS adaptive accumulation
    PAC195X_MODE_1024      = 0x4, ///< 1024 SPS
    PAC195X_MODE_256       = 0x5, ///< 256 SPS
    PAC195X_MODE_64        = 0x6, ///< 64 SPS
    PAC195X_MODE_8         = 0x7, ///< 8 SPS
    PAC195X_MODE_SINGLE    = 0x8, ///< Single-Shot
    PAC195X_MODE_SINGLE_8X = 0x9, ///< Single-Shot 8×
    PAC195X_MODE_FAST      = 0xA, ///< Fast (see DS §5.15.3)
    PAC195X_MODE_BURST     = 0xB, ///< Burst (see DS §5.15.2)
    PAC195X_MODE_SLEEP     = 0xF  ///< Sleep (see DS §5.5)
} pac195X_sample_mode_t;

/**
 * Logical channel number (1–4)
 */
typedef enum
{
    PAC195X_CH1 = 1,
    PAC195X_CH2,
    PAC195X_CH3,
    PAC195X_CH4
} pac195X_channel_t;

#define PAC195X_REG_REFRESH 0x00                              // Send-Byte only, resets accumulators and updates results
#define PAC195X_REG_CTRL 0x01                                 // Control register (sample mode, channel‐off, ALERT/GPIO)
#define PAC195X_REG_VBUS(ch) (uint8_t)(0x07 + ((ch) - 1) * 2) // 2-byte VBUSn result
#define PAC195X_REG_VSENSE(ch) (uint8_t)(0x0B + ((ch) - 1) * 2) // 2-byte VSENSEn result
#define PAC195X_REG_POWER(ch) (uint8_t)(0x17 + ((ch) - 1) * 4)  // 4-byte VPOWERN result
#define PAC195X_REG_REFRESH_V 0x1F    // Send-Byte only, updates results but preserves accumulators
#define PAC195X_REG_ALERT_STATUS 0x26 // 3-byte bitmask of all ALERT conditions

/**
 * Conversion LSBs from datasheet:
 *   • VBUS: 488 µV per LSB (32 V FSR, 16-bit)
 *   • VSENSE: 1.5259 µV per LSB (100 mV FSR, 16-bit)
 * :contentReference[oaicite:10]{index=10}:contentReference[oaicite:11]{index=11}
 */
#define PAC195X_LSB_VBUS (0.000488F)
#define PAC195X_LSB_VSENSE (1.5259e-6F)
#define PAC195X_LSB_POWER (PAC195X_LSB_VBUS * PAC195X_LSB_VSENSE)

/**
 * Runtime handle for one PAC195X device
 */
typedef struct
{
    hw_i2cs_t *i2c;    ///< Pointer to I²C/SMBus controller
    uint8_t    addr;   ///< 7-bit I²C address (ADDRSEL pin config)
    float      rsense; ///< Sense resistor (Ω)
} pac195X_t;

/**
 * @brief Initialize the device and set sample mode
 * @param dev       Pointer to handle (i2c, addr, rsense must be populated)
 * @param mode      One of pac195X_sample_mode_t
 * @return          true on success
 */
bool pac195X_init(pac195X_t *dev, pac195X_sample_mode_t mode);

/**
 * @brief Trigger a full REFRESH: update results & reset accumulators
 */
bool pac195X_refresh(pac195X_t *dev);

/**
 * @brief Trigger REFRESH_V: update results only (preserve accumulators)
 */
bool pac195X_refresh_v(pac195X_t *dev);

/**
 * @brief Read raw bytes from a register with auto-pointer write-then-read
 */
bool pac195X_read_raw(pac195X_t *dev, uint8_t reg, uint8_t *buf, size_t len);

/**
 * @brief Read bus voltage [V] on channel ch
 */
float pac195X_read_voltage(pac195X_t *dev, pac195X_channel_t ch);

/**
 * @brief Read current [A] on channel ch
 */
float pac195X_read_current(pac195X_t *dev, pac195X_channel_t ch);

/**
 * @brief Read power on specified channel
 */
float pac195X_read_power(pac195X_t *dev, pac195X_channel_t ch);

/**
 * @brief Read 24-bit alert status
 */
uint32_t pac195X_read_alert(pac195X_t *dev);
