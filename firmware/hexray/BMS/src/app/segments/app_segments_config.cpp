#include "app_segments.hpp"
#include "app_segments_internal.hpp"

#include <cstring>

using namespace std;

namespace
{
constexpr uint8_t                                            NUM_CONFIG_SYNC_TRIES = 5;
constexpr std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> createSegmentConfig()
{
    std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> config{};
    for (auto &[reg_a, reg_b] : config)
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
    return config;
}
std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> segment_config = createSegmentConfig();
std::array<io::adbms::PWMConfig, NUM_SEGMENTS>     pwm_config{};
expected<bool, ErrorCode>                          isConfigEqual(configa, configb)
{
    const auto segment_config_buf = io::adbms::readConfigReg();

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!segment_config_buf[seg])
        {
            return unexpected(segment_config_buf[seg].error());
        }
        if (segment_config_buf[seg].value() != segment_config[seg])
        {
            return false;
        }
    }
    return true;
}
} // namespace

namespace app::segments::config
{
void setBalanceConfig(const Cells<bool> &balance_config, const bool balancing_enabled)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        auto &[reg_a, reg_b] = segment_config[seg];
        uint16_t dcc_bits    = 0U;

        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            dcc_bits |= static_cast<uint16_t>((balance_config[seg][cell] ? 1U : 0U) << cell);
        }
        reg_a.mute_st  = balancing_enabled;
        reg_b.dcc_1_8  = static_cast<uint8_t>(dcc_bits & 0xFF);
        reg_b.dcc_9_16 = static_cast<uint8_t>(dcc_bits >> 8 & 0xFF);
    }
}

void setPwmConfig(const Cells<uint8_t> &pwm_duty)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        const auto &d   = pwm_duty[seg];
        pwm_config[seg] = {
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

void setThermistorConfig(const ThermistorMux mux)
{
    for (auto &[reg_a, _reg_b] : segment_config)
    {
        UNUSED(_reg_b);
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

expected<void, ErrorCode> configSync()
{
    for (uint8_t tries = 0; tries < NUM_CONFIG_SYNC_TRIES; tries++)
    {
        if (const auto write_ok = io::adbms::writeConfigReg(segment_config); !write_ok)
        {
            continue;
        }
        const auto equal = isConfigEqual();
        if (equal)
        {
            return {};
        }
        LOG_IF_ERR(equal);
    }
    return unexpected(ErrorCode::RETRY_FAILED);
}

expected<void, ErrorCode> upload()
{
    RETURN_IF_ERR(io::adbms::writeConfigReg(segment_config));
    RETURN_IF_ERR(io::adbms::writePwmReg(pwm_config));
    return {};
}
} // namespace app::segments::config
