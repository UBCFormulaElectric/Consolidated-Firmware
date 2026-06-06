#include "app_bootcount.hpp"
#include "io_rtc.hpp"
#include "io_filesystem.hpp"
#include "io_fileSystems.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <expected>
#include <cstdio>

namespace app::sd
{
namespace
{
    constexpr const char *DEFAULT_LOG_PATH = "/boot.bin"; // Fallback path
    std::array<char, 32>  LOG_PATH_BUF{};
    uint32_t              log_fd   = 0;
    bool                  log_open = false;
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
        std::snprintf(
            LOG_PATH_BUF.data(), LOG_PATH_BUF.size(), "/boot_%lu.bin", static_cast<unsigned long>(boot_num.value()));
        log_path = LOG_PATH_BUF.data(); // file name = boot_x.bin where x is the boot number
    }
    else
    {
        return std::unexpected(boot_num.error());
    }

    if (const auto r = fs.open(log_path); r.has_value())
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

    result<io::rtc::Time> rtc_time = io::rtc::get_time();
    result<io::rtc::Date> rtc_date = io::rtc::get_date();

    if (!rtc_time.has_value() || !rtc_date.has_value())
    {
        LOG_ERROR("Failed to get RTC time and/or date");
        return std::unexpected(ErrorCode::ERROR);
    }

    std::array<uint8_t, 7> raw{};
    raw[0] = rtc_time.value().seconds;
    raw[1] = rtc_time.value().minutes;
    raw[2] = rtc_time.value().hours;
    raw[3] = rtc_date.value().day;
    raw[4] = rtc_date.value().weekday;
    raw[5] = rtc_date.value().month;
    raw[6] = rtc_date.value().year;

    const std::span<const uint8_t> metadata_buf(raw.data(), raw.size());
    auto                           err = fs.writeMetadata(log_fd, metadata_buf);

    if (!err.has_value())
        return std::unexpected(ErrorCode::ERROR);

    return {};
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
