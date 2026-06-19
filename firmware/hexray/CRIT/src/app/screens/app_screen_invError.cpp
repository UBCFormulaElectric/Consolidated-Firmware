#include "app_screens.hpp"
#include "io_sevenSeg.hpp"
#include "util_retry.hpp"
#include "app_canRx.hpp"
#include <cassert>

using namespace app::can_rx;

static char screen_buf[io::seven_seg::DIGITS];

static uint16_t prev_fr = 0u;
static uint16_t prev_rr = 0u;
static uint16_t prev_fl = 0u;
static uint16_t prev_rl = 0u;

static void write_error_code(const char pos1, const char pos2, const uint16_t error_code)
{
    screen_buf[0] = pos1;
    screen_buf[1] = pos2;

    uint16_t divisor = 10000u;
    for (uint16_t i = 2; i < 7; i++, divisor /= 10u)
    {
        screen_buf[i] = static_cast<char>('0' + ((error_code / divisor) % 10u));
    }
}

static void update()
{
    const bool     fl_inv_error  = INVFL_bError_get();
    const bool     fr_inv_error  = INVFR_bError_get();
    const bool     rl_inv_error  = INVRL_bError_get();
    const bool     rr_inv_error  = INVRR_bError_get();
    const uint16_t fr_error_code = INVFR_ErrorInfo_get();
    const uint16_t rr_error_code = INVRR_ErrorInfo_get();
    const uint16_t fl_error_code = INVFL_ErrorInfo_get();
    const uint16_t rl_error_code = INVRL_ErrorInfo_get();

    // priority of error codes is based on which wheels travel longer dist:
    // 1. fr
    // 2. rr
    // 3. fl
    // 4. rl
    // If higher priority error code is on we'll just keep that on first two segs are the
    // inverter last 5 the code from CAN

    if (fr_inv_error && fr_error_code != prev_fr)
    {
        write_error_code('f', 'r', fr_error_code);
        prev_fr = fr_error_code;
    }
    else if (rr_inv_error && rr_error_code != prev_rr)
    {
        write_error_code('r', 'r', rr_error_code);
        prev_rr = rr_error_code;
    }
    else if (fl_inv_error && fl_error_code != prev_fl)
    {
        write_error_code('f', 'l', fl_error_code);
        prev_fl = fl_error_code;
    }
    else if (rl_inv_error && rl_error_code != prev_rl)
    {
        write_error_code('r', 'l', rl_error_code);
        prev_rl = rl_error_code;
    }
    const auto screen_write_result = util::retry([&] { return io::seven_seg::write(screen_buf); }, 3);
    assert(screen_write_result.has_value());
}

constexpr app::screens::Screen app::screens::inv_error_screen = {
    [] {},
    [] {},
    update,
};
