#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "app_canAlerts.hpp"
#include "util_retry.hpp"
#include "app_canRx.hpp"
#include <algorithm>
#include <cassert>

static int     alert_index = 0;
static uint8_t alert_count = 0;
using namespace app::can_rx;
static void update()
{
    //
    std::array<io::seven_seg::digit, io::seven_seg::DIGITS> screen_buf{
        { io::seven_seg::a, io::seven_seg::l, io::seven_seg::t, io::seven_seg::dot, io::seven_seg::dot,
          io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot }
    };

    std::array<app::can_alerts::Alert_Info, 20> buf{};
    const bool                                  fl_inv_error = INVFL_bError_get();
    const bool                                  fr_inv_error = INVFR_bError_get();
    const bool                                  rl_inv_error = INVRL_bError_get();
    const bool                                  rr_inv_error = INVFR_bError_get();
    // priority of error codes is based on which wheels travel longer dist:
    // 1. fr
    // 2. rr
    // 3. fl
    // 4. rl
    // If higher priority error code is on we'll just keep that on first two segs are the
    // inverter last 5 the code from CAN

    if (fr_inv_error || fl_inv_error || rr_inv_error || rl_inv_error)
    {
        if (fr_inv_error)
        {
            screen_buf[0]             = io::seven_seg::f;
            screen_buf[1]             = io::seven_seg::r;
            const uint16_t error_code = INVFR_ErrorInfo_get();
            uint16_t       divisor    = 10000u;
            for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
            {
                screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
            }
        }
        else if (rr_inv_error)
        {
            screen_buf[0]             = io::seven_seg::r;
            screen_buf[1]             = io::seven_seg::r;
            const uint16_t error_code = INVRR_ErrorInfo_get();
            uint16_t       divisor    = 10000u;
            for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
            {
                screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
            }
        }
        else if (fl_inv_error)
        {
            screen_buf[0]             = io::seven_seg::f;
            screen_buf[1]             = io::seven_seg::l;
            const uint16_t error_code = INVFL_ErrorInfo_get();
            uint16_t       divisor    = 10000u;
            for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
            {
                screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
            }
        }
        else
        {
            screen_buf[0]             = io::seven_seg::r;
            screen_buf[1]             = io::seven_seg::l;
            const uint16_t error_code = INVRL_ErrorInfo_get();
            uint16_t       divisor    = 10000u;
            for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
            {
                screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
            }
        }
        goto write;
    }

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
    const auto screen_write_result = util::retry([&] { return io::seven_seg::write(screen_buf); }, 3);
    assert(screen_write_result.has_value());
}

constexpr app::screens::Screen app::screens::alerts_screen = {
    [] { alert_index = std::clamp(alert_index + 1, 0, alert_count - 1); },
    [] { alert_index                                       = std::clamp(alert_index - 1, 0, alert_count - 1); },
    update,
};
