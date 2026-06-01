#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "util_retry.hpp"

#include <cassert>
#include <cstring>
#include <atomic>
#include <algorithm>

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

constexpr std::array<io::adbms::PWMConfig, NUM_SEGMENTS> createPwmConfig()
{
    std::array<io::adbms::PWMConfig, NUM_SEGMENTS> config{};
    for (auto &[_reg_a, reg_b] : config)
        reg_b.res = 0xFFFFFFFFu;
    return config;
}

std::array<io::adbms::SegmentConfig, NUM_SEGMENTS> segment_config = createSegmentConfig();
std::array<io::adbms::PWMConfig, NUM_SEGMENTS>     pwm_config     = createPwmConfig();
bool                                               dirty          = true;

io::semaphore config_data_lock{ true}; // protects the segment_config and pwm_config arrays, and dirty bit

/**
 * @return per-segment equality status. For each segment: value()==true if the ADBMS config matches
 * the in-memory config, value()==false if it's mismatched, or an error (e.g., CHECKSUM_FAIL) if we
 * couldn't read that segment's config.
 * @note Caller must hold config_data_lock.
 */
io::adbms::Segments<result<bool>> isConfigEqual()
{
    assert(config_data_lock.is_held());
    const auto segment_config_buf = io::adbms::read::configReg();
    const auto pwm_config_buf     = io::adbms::read::pwmReg();

    io::adbms::Segments<result<bool>> out;
    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!segment_config_buf[seg])
        {
            out[seg] = unexpected(segment_config_buf[seg].error());
            continue;
        }
        if (!pwm_config_buf[seg])
        {
            out[seg] = unexpected(pwm_config_buf[seg].error());
            continue;
        }
        const auto sc_read = segment_config_buf[seg].value();
        const auto pc_read = pwm_config_buf[seg].value();
        const bool sc_eq   = (sc_read == segment_config[seg]);
        const bool pc_eq   = (pc_read == pwm_config[seg]);
        out[seg]           = sc_eq && pc_eq;
    }
    return out;
}

/**
 * Writes the config_data arrays in this file to the ADBMS. If an error occurs during writing, returns that error.
 * @return success of operation
 * @note Caller must hold config_data_lock.
 */
result<void> upload()
{
    assert(config_data_lock.is_held());
    RETURN_IF_ERR_SILENT(io::adbms::write::configReg(segment_config));
    RETURN_IF_ERR_SILENT(io::adbms::write::pwmReg(pwm_config));
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
            .reg_b = { static_cast<uint8_t>(d[12] & 0x0F), static_cast<uint8_t>(d[13] & 0x0F), 0, 0, 0xFFFFFFFFu },
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
        reg_a.gpio_9_10 = 0x2 | (mux == ThermistorMux::THERMISTOR_MUX_8_13);
    }
    dirty = true;
}

Segments<result<bool>> sync()
{
    const io::unique_semaphore lock{ config_data_lock };

    // Fast path: if nothing's marked dirty, double-check the chip still matches in case of a reset.
    if (not dirty)
    {
        if (const Segments<result<bool>> seg_configs_equal = isConfigEqual(); ranges::all_of(
                seg_configs_equal, [](const auto &seg_ok_res) -> bool { return seg_ok_res and seg_ok_res.value(); }))
        {
            return seg_configs_equal;
        }
        dirty = true;
    }
    assert(dirty); // hence, we will resync

    // Slow path: write config to the chip and verify, retrying up to NUM_CONFIG_SYNC_TRIES times.
    // The retry result is discarded — per-segment errors are recorded in seg_had_error and
    // reflected in the Config health bit.
    Segments<result<bool>> per_seg;
    for (size_t attempt = 0; attempt < NUM_CONFIG_SYNC_TRIES; ++attempt)
    {
        if (const result<void> up = upload(); !up)
        {
            // Daisy-chain SPI failure: leaves every segment in an unknown state.
            per_seg.fill(unexpected(up.error()));
            continue;
        }
        if (per_seg = isConfigEqual(); ranges::any_of(
                per_seg, [](const result<bool> &seg_ok_res) -> bool { return not(seg_ok_res and seg_ok_res.value()); }))
        {
            // comes here if 1+ segments are not equal, or failed to read back (e.g., due to a CHECKSUM_FAIL)
            continue;
        }
        dirty = false;
        LOG_INFO("Config Sync Success on attempt %d", attempt);
        break;
    }
    return per_seg;
}
} // namespace app::segments::config
