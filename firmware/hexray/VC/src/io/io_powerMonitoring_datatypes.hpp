#pragma once
#include <cstdint>

constexpr uint8_t CHANNEL_NUM = 4;
// commands
constexpr uint8_t REG_REFRESH = 0x00;

// register addresses (pg 38.)
constexpr uint16_t REG_CTRL         = 0x01;
constexpr uint16_t REG_VBUS         = 0x07;
constexpr uint16_t REG_VSENSE       = 0x0B;
constexpr uint16_t REG_VPOWERN      = 0x17;
constexpr uint16_t REG_NEG_PWR_FSR  = 0x1D;
constexpr uint16_t REG_ACCUM_CONFIG = 0x25;

// protection addresses
constexpr uint16_t REG_PROTECTION_OV = 0x3C;
constexpr uint16_t REG_PROTECTION_UV = 0x40;
constexpr uint16_t ALERT_EN          = 0x49;
constexpr uint16_t REG_ALERT_STATUS  = 0x26;
constexpr uint16_t REG_SLOW_ALERT1   = 0x27;

// bitmask: OV (bits 0,1, 2, 3), UV (bits 4, 5, 6, 7)
enum AlertOvUvBits : uint8_t
{
    ALERT_OV_CH1 = (1u << 0),
    ALERT_OV_CH2 = (1u << 1),
    ALERT_OV_CH3 = (1u << 2),
    ALERT_OV_CH4 = (1u << 3),
    ALERT_UV_CH1 = (1u << 4),
    ALERT_UV_CH2 = (1u << 5),
    ALERT_UV_CH3 = (1u << 6),
    ALERT_UV_CH4 = (1u << 7),
};
