#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "app_canAlerts.hpp"
#include "util_retry.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>

static int     alert_index = 0;
static uint8_t alert_count = 0;
static void    update()
{
    char screen_buf[io::seven_seg::DIGITS];

    std::array<app::can_alerts::Alert_Info, 20> buf{};

    if (const uint8_t fault_count = app::can_alerts::FaultInfo(buf); fault_count > 0)
    {
        alert_count = fault_count;
        snprintf(screen_buf, io::seven_seg::DIGITS, "altflt");
        // todo fault code
        goto write;
    }
    if (const uint8_t warning_count = app::can_alerts::WarningInfo(buf); warning_count > 0)
    {
        alert_count = warning_count;
        snprintf(screen_buf, io::seven_seg::DIGITS, "altwrn");
        // todo warning code
        goto write;
    }
    LOG_ERROR("why am in this page");
    return;

write:
    const auto screen_write_result = util::retry([&] { return io::seven_seg::write(screen_buf); }, 3);
    assert(screen_write_result.has_value());
}

constexpr app::screens::Screen app::screens::alerts_screen = {
    [] { alert_index = std::clamp(alert_index + 1, 0, alert_count - 1); },
    [] { alert_index                                       = std::clamp(alert_index - 1, 0, alert_count - 1); },
    update,
};
