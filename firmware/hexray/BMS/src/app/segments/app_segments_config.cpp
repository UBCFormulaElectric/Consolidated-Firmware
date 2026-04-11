#include "app_segments.hpp"
#include "app_segments_internal.hpp"

#include <cstring>

using namespace std;

inline constexpr uint8_t  NUM_CONFIG_SYNC_TRIES = 20;
inline constexpr uint16_t VUV                   = 0x800; // VUV × 16 × 150 μV + 1.5 V (TO DO)
inline constexpr uint16_t VOV                   = 0x7FF; // VOV × 16 × 150 μV + 1.5 V (TO DO)

static array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> segment_config;
static array<io::adbms::PWMConfig, io::NUM_SEGMENTS>     segment_pwm_config;

namespace app::segments
{
void setDefaultConfig()
{
    for (auto &[reg_a, reg_b] : segment_config)
    {
        reg_a.cth       = 0x01;
        reg_a.ref_on    = 0x01;
        reg_a.gpio_1_8  = 0xFF;
        reg_a.gpio_9_10 = 0x03;
        reg_a.fc        = 0x03;

        reg_b.vuv_0_7  = static_cast<uint8_t>(VUV & 0xFF);
        reg_b.vuv_8_11 = static_cast<uint8_t>(VUV >> 8 & 0x0F);
        reg_b.vov_0_3  = static_cast<uint8_t>(VOV & 0x0F);
        reg_b.vov_4_11 = static_cast<uint8_t>(VOV >> 4 & 0xFF);
    }
}

void setBalanceConfig(
    array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &balance_config,
    bool                                                         balancing_enabled)
{
    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        auto &[reg_a, reg_b] = segment_config[seg];
        uint16_t dcc_bits    = 0U;

        (void)reg_a;
        for (uint8_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            dcc_bits |= static_cast<uint16_t>((balance_config[seg][cell] ? 1U : 0U) << cell);
        }
        if (balancing_enabled)
        {
            reg_a.mute_st = 0x00;
        }
        else
        {
            reg_a.mute_st = 0x01;
        }

        reg_b.dcc_1_8  = static_cast<uint8_t>(dcc_bits & 0xFF);
        reg_b.dcc_9_16 = static_cast<uint8_t>(dcc_bits >> 8 & 0xFF);
    }
}

void setPwmConfig(array<array<uint8_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &pwm_duty)
{
    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        const auto &d           = pwm_duty[seg];
        segment_pwm_config[seg] = {
            .reg_a = { static_cast<uint8_t>(d[0] & 0x0F), static_cast<uint8_t>(d[1] & 0x0F),
                       static_cast<uint8_t>(d[2] & 0x0F), static_cast<uint8_t>(d[3] & 0x0F),
                       static_cast<uint8_t>(d[4] & 0x0F), static_cast<uint8_t>(d[5] & 0x0F),
                       static_cast<uint8_t>(d[6] & 0x0F), static_cast<uint8_t>(d[7] & 0x0F),
                       static_cast<uint8_t>(d[8] & 0x0F), static_cast<uint8_t>(d[9] & 0x0F),
                       static_cast<uint8_t>(d[10] & 0x0F), static_cast<uint8_t>(d[11] & 0x0F) },
            .reg_b = { static_cast<uint8_t>(d[12] & 0x0F), static_cast<uint8_t>(d[13] & 0x0F), 0, 0 },
        };
    }
}

void setThermistorConfig(ThermistorMux mux)
{
    for (auto &[reg_a, reg_b] : segment_config)
    {
        (void)reg_b;

        if (mux == ThermistorMux::THERMISTOR_MUX_0_7)
        {
            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x02;
        }
        else
        {
            reg_a.gpio_1_8  = 0xFF;
            reg_a.gpio_9_10 = 0x03;
        }
    }
}

static expected<void, ErrorCode> isConfigEqual()
{
    array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>  segment_config_buf;
    array<expected<void, ErrorCode>, io::NUM_SEGMENTS> segment_success_buf;

    io::adbms::readConfigReg(segment_config_buf, segment_success_buf);

    for (uint8_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        if (!segment_success_buf[seg])
        {
            return segment_success_buf[seg];
        }
        else
        {
            if (memcmp(&segment_config_buf[seg], &segment_config[seg], sizeof(segment_config[seg])) != 0)
            {
                return unexpected(ErrorCode::CHECKSUM_FAIL);
            }
        }
    }
    return {};
}

expected<void, ErrorCode> configSync()
{
    for (uint8_t tries = 0; tries < NUM_CONFIG_SYNC_TRIES; tries++)
    {
        const auto write_ok = io::adbms::writeConfigReg(segment_config);
        if (!write_ok)
        {
            continue;
        }
        else
        {
            const auto equal = isConfigEqual();
            if (equal)
            {
                return {};
            }
        }
    }
    return unexpected(ErrorCode::RETRY_FAILED);
}

expected<void, ErrorCode> writeConfig()
{
    RETURN_IF_ERR(io::adbms::writeConfigReg(segment_config));
    RETURN_IF_ERR(io::adbms::writePwmReg(segment_pwm_config));
    return {};
}

} // namespace app::segments
