#include "app_bootcount.hpp"
#include "io_rtc.hpp"
#include "io_filesystem.hpp"
#include "io_fileSystems.hpp"

#include <array>
#include <cstdio>

namespace app::sd
{
namespace
{
    constexpr const char *DEFAULT_LOG_PATH = "/log.bin"; // Fallback path
    std::array<char, 32> LOG_PATH_BUF{};
    uint32_t             log_fd   = 0;
    bool                 log_open = false;
} // namespace

void init_fs()
{
    if (log_open)
        return;

    if (const auto err = fs.init(); not err)
    {
        LOG_ERROR("Failed to init filesystem: %d", static_cast<int>(err.error()));
    }
    else
    {
        LOG_INFO("Initialised filesystem successfully.");
    }

    const char *log_path = DEFAULT_LOG_PATH;
    if (const auto boot_num = app::bootcount::update(fs); boot_num)
    {
        std::snprintf(LOG_PATH_BUF.data(), LOG_PATH_BUF.size(), "/boot_%lu.bin",
                      static_cast<unsigned long>(boot_num.value()));
        log_path = LOG_PATH_BUF.data();
    }
    else
    {
        LOG_ERROR("Failed to update bootcount: %d", static_cast<int>(boot_num.error()));
    }

    if (const auto r = fs.open(log_path); r)
    {
        log_fd   = r.value();
        log_open = true;
    }
    else
    {
        LOG_ERROR("Failed to open %s: %d", log_path, static_cast<int>(r.error()));
    }
}

void update_metadata()
{
    result<io::rtc::Time> rtc_time = io::rtc::get_time();
    result<io::rtc::Date> rtc_date = io::rtc::get_date();

    if (!rtc_time || !rtc_date)
    {
        LOG_ERROR("Failed to get RTC time and/or date");
        return;
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
        LOG_ERROR("Failed to write time to file metadata: %d", static_cast<int>(err.error()));
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
