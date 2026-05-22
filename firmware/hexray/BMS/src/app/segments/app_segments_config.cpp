#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "util_retry.hpp"
#include "hw_notify.hpp"

#include <cassert>
#include <cstring>
#include <atomic>
#include <optional>

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
        out[seg] = (segment_config_buf[seg].value() == segment_config[seg]) &&
                   (pwm_config_buf[seg].value() == pwm_config[seg]);
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
    RETURN_IF_ERR(io::adbms::write::configReg(segment_config));
    RETURN_IF_ERR(io::adbms::write::pwmReg(pwm_config));
    return {};
}

/**
 * Folds per-segment equality results into the sticky error accumulator and returns whether every
 * segment is healthy and equal.
 */
bool absorbAndCheckAllEqual(
    const io::adbms::Segments<result<bool>> &per_seg,
    io::adbms::Segments<bool>               &seg_had_error)
{
    bool all_equal = true;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        if (!per_seg[seg] || !per_seg[seg].value())
        {
            seg_had_error[seg] = true;
            all_equal          = false;
        }
    }
    return all_equal;
}

/**
 * Returns the first per-segment read error in @p per_seg, if any.
 */
std::optional<ErrorCode> firstReadError(const io::adbms::Segments<result<bool>> &per_seg)
{
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        if (!per_seg[seg]) return per_seg[seg].error();
    return std::nullopt;
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

void configSync()
{
    const io::unique_semaphore lock{ config_data_lock };

    // Sticky per-segment error accumulator for this sync call: a segment is flagged if it had any
    // read error (e.g., CHECKSUM_FAIL) or was mismatched at any point during this call.
    Segments<bool> seg_had_error{};

    const auto writeHealthBits = [&]() {
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
            health::setOrReset(seg, health::ErrorBit::Config, seg_had_error[seg]);
    };

    // Fast path: if nothing's marked dirty, double-check the chip still matches in case of a reset.
    if (not dirty)
    {
        if (absorbAndCheckAllEqual(isConfigEqual(), seg_had_error))
        {
            writeHealthBits();
            sync_done.notifyIfWaiting();
            return;
        }
        dirty = true;
    }

    // Slow path: write config to the chip and verify, retrying up to NUM_CONFIG_SYNC_TRIES times.
    // The retry result is discarded — per-segment errors are recorded in seg_had_error and
    // reflected in the Config health bit.
    const auto r = util::retry(
        [&]() -> result<void>
        {
            const auto up = upload();
            if (!up)
            {
                // Daisy-chain SPI failure: leaves every segment in an unknown state.
                for (auto &flag : seg_had_error) flag = true;
                return unexpected(up.error());
            }

            const auto per_seg = isConfigEqual();
            if (absorbAndCheckAllEqual(per_seg, seg_had_error))
            {
                dirty = false;
                return {};
            }

            // Drive the retry with a meaningful error: prefer the per-segment read error over a
            // plain mismatch.
            if (const auto err = firstReadError(per_seg)) return unexpected(*err);
            return unexpected(ErrorCode::RETRY_FAILED);
        },
        NUM_CONFIG_SYNC_TRIES);

    writeHealthBits();
    if (r) sync_done.notifyIfWaiting();
}

result<void> waitForSync(const uint32_t timeout_ms)
{
    const auto r = sync_done.waitFor(timeout_ms);
    if (!r && r.error() == ErrorCode::TIMEOUT)
        return std::unexpected(ErrorCode::CONFIG_TIMEOUT);
    return r;
}

} // namespace app::segments::config
