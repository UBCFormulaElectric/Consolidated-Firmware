#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "app_canAlerts.hpp"

#include <algorithm>

static int     alert_index = 0;
static uint8_t alert_count = 0;

static void update()
{
    std::array<io::seven_seg::digit, io::seven_seg::DIGITS> screen_buf{
        { io::seven_seg::a, io::seven_seg::l, io::seven_seg::t, io::seven_seg::dot, io::seven_seg::dot,
          io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot }
    };

    std::array<app::can_alerts::Alert_Info, 20> buf;

    if (const uint8_t fault_count = app::can_alerts::FaultInfo(buf); fault_count > 0)
    {
        alert_count   = fault_count;
        screen_buf[3] = io::seven_seg::f;
        screen_buf[4] = io::seven_seg::l;
        screen_buf[5] = io::seven_seg::t;
        // todo fault code
        goto write;
    }
    if (const uint8_t warning_count = app::can_alerts::WarningInfo(buf); warning_count > 0)
    {
        alert_count   = warning_count;
        screen_buf[3] = io::seven_seg::u;
        screen_buf[4] = io::seven_seg::r;
        screen_buf[5] = io::seven_seg::n;
        // todo warning code
        goto write;
    }
    LOG_ERROR("why am in this page");
    return;

write:
    io::seven_seg::write(screen_buf);
}

constexpr app::screens::Screen app::screens::alerts_screen = {
    [] { alert_index = std::clamp(alert_index + 1, 0, alert_count - 1); },
    [] { alert_index                                       = std::clamp(alert_index - 1, 0, alert_count - 1); },
    update,
};
