#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "util_retry.hpp"

#include <cstring>

using namespace std;

namespace
{
constexpr uint8_t NUM_CONFIG_SYNC_TRIES = 5;

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

io::semaphore config_data_lock{ true };

// Caller must hold config_data_lock.
expected<bool, ErrorCode> isConfigEqual_locked()
{
    const auto segment_config_buf = io::adbms::readConfigReg();
    const auto pwm_config_buf     = io::adbms::readPwmReg();

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!segment_config_buf[seg])
            return unexpected(segment_config_buf[seg].error());
        if (!pwm_config_buf[seg])
            return unexpected(pwm_config_buf[seg].error());
        if (segment_config_buf[seg].value() != segment_config[seg])
            return false;
        if (pwm_config_buf[seg].value() != pwm_config[seg])
            return false;
    }
    return true;
}

// Caller must hold config_data_lock.
expected<void, ErrorCode> upload_locked()
{
    RETURN_IF_ERR(io::adbms::writeConfigReg(segment_config));
    RETURN_IF_ERR(io::adbms::writePwmReg(pwm_config));
    return {};
}

} // namespace

namespace app::segments::config
{
void setBalanceConfig(const Cells<bool> &balance_config, const Cells<uint8_t> &pwm_duty, const bool balancing_enabled)
{
    const io::unique_semaphore lock{ config_data_lock };
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        auto &[reg_a, reg_b] = segment_config[seg];

        uint16_t dcc_bits = 0U;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            dcc_bits |= static_cast<uint16_t>(static_cast<uint16_t>(balance_config[seg][cell]) << cell);

        reg_a.mute_st  = balancing_enabled;
        reg_b.dcc_1_8  = static_cast<uint8_t>(dcc_bits);
        reg_b.dcc_9_16 = static_cast<uint8_t>(dcc_bits >> 8);

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

expected<void, ErrorCode> setThermistorConfig(const ThermistorMux mux)
{
    const io::unique_semaphore lock{ config_data_lock };
    for (auto &cfg : segment_config)
    {
        cfg.reg_a.gpio_1_8  = 0xFF;
        cfg.reg_a.gpio_9_10 = (mux == ThermistorMux::THERMISTOR_MUX_0_7) ? 0x02 : 0x03;
    }
    return io::adbms::writeConfigReg(segment_config);
}

expected<void, ErrorCode> configSync()
{
    const io::unique_semaphore lock{ config_data_lock };

    const auto already_equal = isConfigEqual_locked();
    RETURN_IF_ERR(already_equal);
    if (already_equal.value())
        return {};

    return util::retry(
        []() -> expected<void, ErrorCode>
        {
            RETURN_IF_ERR(upload_locked());
            const auto equal = isConfigEqual_locked();
            RETURN_IF_ERR(equal);
            return equal.value() ? expected<void, ErrorCode>{} : unexpected(ErrorCode::RETRY_FAILED);
        },
        NUM_CONFIG_SYNC_TRIES);
}

} // namespace app::segments::config
