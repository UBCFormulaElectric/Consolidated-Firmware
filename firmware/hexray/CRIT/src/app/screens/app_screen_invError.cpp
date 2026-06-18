#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "util_retry.hpp"
#include "app_canRx.hpp"
#include <algorithm>
#include <cassert>

std::array<io::seven_seg::digit, io::seven_seg::DIGITS> screen_buf{
    { io::seven_seg::a, io::seven_seg::l, io::seven_seg::t, io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot,
      io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot }
};

static uint16_t prev_fr = 0u;
static uint16_t prev_rr = 0u;
static uint16_t prev_fl = 0u;
static uint16_t prev_rl = 0u;

static void update()
{
    const bool fl_inv_error = INVFL_bError_get();
    const bool fr_inv_error = INVFR_bError_get();
    const bool rl_inv_error = INVRL_bError_get();
    const bool rr_inv_error = INVRR_bError_get();

    // priority of error codes is based on which wheels travel longer dist:
    // 1. fr
    // 2. rr
    // 3. fl
    // 4. rl
    // If higher priority error code is on we'll just keep that on first two segs are the
    // inverter last 5 the code from CAN

    if (fr_inv_error && fr_inv_error != prev_fr)
    {
        screen_buf[0]             = io::seven_seg::f;
        screen_buf[1]             = io::seven_seg::r;
        const uint16_t error_code = INVFR_ErrorInfo_get();
        uint16_t       divisor    = 10000u;
        for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
        {
            screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
        }
        prev_fr = error_code;
    }
    else if (rr_inv_error && rr_inv_error != prev_rr)
    {
        screen_buf[0]             = io::seven_seg::r;
        screen_buf[1]             = io::seven_seg::r;
        const uint16_t error_code = INVRR_ErrorInfo_get();
        uint16_t       divisor    = 10000u;
        for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
        {
            screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
        }
        prev_rr = error_code;
    }
    else if (fl_inv_error && fl_inv_error != prev_fl)
    {
        screen_buf[0]             = io::seven_seg::f;
        screen_buf[1]             = io::seven_seg::l;
        const uint16_t error_code = INVFL_ErrorInfo_get();
        uint16_t       divisor    = 10000u;
        for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
        {
            screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
        }
        prev_fl = error_code;
    }
    else if (rl_inv_error && rl_inv_error != prev_rl)
    {
        screen_buf[0]             = io::seven_seg::r;
        screen_buf[1]             = io::seven_seg::l;
        const uint16_t error_code = INVRL_ErrorInfo_get();
        uint16_t       divisor    = 10000u;
        for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
        {
            screen_buf[i] = io::seven_seg::digit_to_segment(static_cast<uint8_t>((error_code / divisor) % 10u));
        }
        prev_rl = error_code;
    }
    const auto screen_write_result = util::retry([&] { return io::seven_seg::write(screen_buf); }, 3);
    assert(screen_write_result.has_value());
}

constexpr app::screens::Screen app::screens::inv_error_screen = {
    [] {},
    [] {},
    update,
};
