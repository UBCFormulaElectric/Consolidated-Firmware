#include "io_rtc.hpp"
#include "io_filesystem.hpp"
#include "io_filesystems.hpp"

namespace app::sd
{
namespace
{
auto     LOG_PATH = "/log.bin";
uint32_t log_fd   = 0;
bool     log_open = false;
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

    if (const auto r = fs.open(LOG_PATH); r)
    {
        log_fd   = r.value();
        log_open = true;
    }
    else
    {
        LOG_ERROR("Failed to open %s: %d", LOG_PATH, static_cast<int>(r.error()));
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

    std::array<uint8_t, 6> raw{};
    raw[0] = rtc_date.value().month;
    raw[1] = rtc_date.value().day;
    raw[2] = rtc_date.value().year;
    raw[3] = rtc_time.value().hours;
    raw[4] = rtc_time.value().minutes;
    raw[5] = rtc_time.value().seconds;

    const std::span<const uint8_t> metadata_buf(raw.data(), raw.size());
    auto err = fs.writeMetadata(log_fd, metadata_buf);

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
