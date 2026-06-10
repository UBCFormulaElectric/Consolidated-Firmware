#include "app_bootcount.hpp"
#include "app_crc32.hpp"
#include "app_epochClock.hpp"
#include "io_filesystem.hpp"
#include "io_fileSystems.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <atomic>
#include <expected>
#include <cstdio>

namespace app::sd
{
namespace
{
    constexpr const char *DEFAULT_LOG_PATH = "/boot.bin"; // Fallback path
    std::array<char, 48>  LOG_PATH_BUF{};
    uint32_t              log_fd   = 0;
    bool                  log_open = false;
    std::atomic<bool>     metadata_update_requested{ false };
    std::atomic<bool>     sync_requested{ false };

    [[nodiscard]] uint32_t makeBootFilenameHash(uint32_t boot_num) noexcept
    {
        const auto now_ms = io::time::getCurrentMs();
        char       scratch[32]{};
        const auto len = std::snprintf(
            scratch, sizeof(scratch), "%lu_%lu", static_cast<unsigned long>(boot_num),
            static_cast<unsigned long>(now_ms));
        if (len < 0)
            return 0u;
        return app::crc32::finalize(app::crc32::update(app::crc32::init(), scratch, static_cast<size_t>(len)));
    }

    [[nodiscard]] const char *formatBootFilename(uint32_t boot_num, uint32_t hash) noexcept
    {
        std::snprintf(
            LOG_PATH_BUF.data(), LOG_PATH_BUF.size(), "/boot_%lu_%08lx.bin", static_cast<unsigned long>(boot_num),
            static_cast<unsigned long>(hash));
        return LOG_PATH_BUF.data();
    }
} // namespace

std::expected<void, io::FileSystem::FileSystemError> init_fs()
{
    if (log_open)
        return {};

    if (const auto err = fs.init(); !err.has_value())
        return std::unexpected(err.error());

    const char *log_path = DEFAULT_LOG_PATH;
    if (const auto boot_num = app::bootcount::update(fs); boot_num) // update bootcount
    {
        const auto filename_hash = makeBootFilenameHash(static_cast<uint32_t>(boot_num.value()));
        log_path                 = formatBootFilename(static_cast<uint32_t>(boot_num.value()), filename_hash);
    }
    else
    {
        return std::unexpected(boot_num.error());
    }

    if (const auto r = fs.open(log_path); r.has_value()) // filename: boot_{boot_num}_{hash}.bin
    {
        log_fd   = r.value();
        log_open = true;
        return {};
    }
    else
    {
        return std::unexpected(r.error());
    }
}

std::expected<void, ErrorCode> update_metadata()
{
    if (!log_open)
        return std::unexpected(ErrorCode::INVALID_ARGS);

    const auto epoch_ms = app::epochClock::getEpochMsFast();
    if (!epoch_ms.has_value())
    {
        LOG_ERROR("Failed to get current epoch ms");
        return std::unexpected(ErrorCode::ERROR);
    }

    // Update basetime: basetime = ntp_time - ms_since_boot
    const uint64_t ms_since_boot = io::time::getCurrentMs();
    const uint64_t base_time     = epoch_ms.value() - ms_since_boot;

    const auto dt = app::epochClock::dateTimeFromEpoch(base_time);
    if (!dt)
    {
        LOG_ERROR("Failed to convert basetime to DateTime");
        return std::unexpected(ErrorCode::ERROR);
    }

    const auto wkday = app::epochClock::weekdayFromEpoch(base_time);
    if (!wkday)
    {
        LOG_ERROR("Failed to compute weekday from base_time");
        return std::unexpected(ErrorCode::ERROR);
    }

    std::array<uint8_t, 7> raw{};
    raw[0] = static_cast<uint8_t>(dt->seconds);
    raw[1] = static_cast<uint8_t>(dt->minutes);
    raw[2] = static_cast<uint8_t>(dt->hours);
    raw[3] = static_cast<uint8_t>(dt->day);
    raw[4] = wkday.value();
    raw[5] = static_cast<uint8_t>(dt->month);
    raw[6] = static_cast<uint8_t>(dt->year - 2000);

    const std::span<const uint8_t> metadata_buf(raw.data(), raw.size());
    auto                           err = fs.writeMetadata(log_fd, metadata_buf);

    if (!err.has_value())
        return std::unexpected(ErrorCode::ERROR);

    return {};
}

void requestMetadataUpdate()
{
    metadata_update_requested.store(true, std::memory_order_relaxed);
}

void requestSync()
{
    sync_requested.store(true, std::memory_order_relaxed);
}

void service()
{
    if (metadata_update_requested.exchange(false, std::memory_order_relaxed))
    {
        if (const auto err = update_metadata(); !err.has_value())
            LOG_ERROR("sd service: metadata update failed: %d", static_cast<int>(err.error()));
    }
    if (sync_requested.exchange(false, std::memory_order_relaxed) && log_open)
    {
        if (const auto err = fs.sync(log_fd); !err.has_value())
            LOG_ERROR("sd service: sync failed: %d", static_cast<int>(err.error()));
    }
}

uint32_t getLogFd()
{
    return log_fd;
}

bool isLogOpen()
{
    return log_open;
}
} // namespace app::sd
