#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "util_retry.hpp"
#include "hw_notify.hpp"

#include <cassert>
#include <cstring>
#include <atomic>

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
bool                                               dirty = true;

io::semaphore config_data_lock{ true }; // protects the segment_config and pwm_config arrays, and dirty bit

/**
 * @return if the config on the ADBMS matches the config_data arrays in this file. If an error occurs while reading the
 * config from the ADBMS, returns that error.
 * @note Caller must hold config_data_lock.
 */
result<bool> isConfigEqual()
{
    assert(config_data_lock.is_held());
    const auto segment_config_buf = io::adbms::read::configReg();
    const auto pwm_config_buf     = io::adbms::read::pwmReg();

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

/**
 * Writes the config_data arrays in this file to the ADBMS. If an error occurs during writing, returns that error.
 * @return success of operation
 * @note Caller must hold config_data_lock.
 */
result<void> upload()
{
    assert(config_data_lock.is_held());
    RETURN_IF_ERR(io::adbms::write::configReg(segment_config));
    RETURN_IF_ERR(io::adbms::write::pwmReg(pwm_config));
    return {};
}

} // namespace

namespace app::segments::config
{
hw::notify::Notifier sync_done;

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
    dirty = true;
}

void setThermistorConfig(const ThermistorMux mux)
{
    const io::unique_semaphore lock{ config_data_lock };
    for (auto &[reg_a, _reg_b] : segment_config)
    {
        reg_a.gpio_1_8  = 0xFF;
        reg_a.gpio_9_10 = mux == ThermistorMux::THERMISTOR_MUX_0_7 ? 0x02 : 0x03;
    }
    dirty = true;
}

result<void> configSync()
{
    const io::unique_semaphore lock{ config_data_lock };

    if (not dirty)
    {
        const auto config_equal = isConfigEqual();
        RETURN_IF_ERR(config_equal);
        dirty |= !config_equal.value();
    }

    if (not dirty)
    {
        sync_done.notifyIfWaiting();
        return {};
    }

    const auto r = util::retry(
        []() -> result<void>
        {
            RETURN_IF_ERR(upload());
            const auto equal = isConfigEqual();
            RETURN_IF_ERR(equal);
            if (!equal.value()) return unexpected(ErrorCode::RETRY_FAILED);
            dirty = false;
            return {};
        },
        NUM_CONFIG_SYNC_TRIES);
    if (r)
        sync_done.notifyIfWaiting();
    return r;
}

result<void> waitForSync(const uint32_t timeout_ms)
{
    const auto r = sync_done.waitFor(timeout_ms);
    if (!r && r.error() == ErrorCode::TIMEOUT)
        return std::unexpected(ErrorCode::CONFIG_TIMEOUT);
    return r;
}

} // namespace app::segments::config
