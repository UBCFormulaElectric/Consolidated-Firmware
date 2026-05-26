#include "app_tsim.hpp"

#include "app_canRx.hpp"
#include "app_timer.hpp"
#include "io_tsim.hpp"

namespace app::tsim
{
namespace
{
    constexpr uint32_t RED_TOGGLE_TIME_MS    = 150;
    constexpr uint32_t BOOTUP_IGNORE_TIME_MS = 3000;

    enum class TsimColor
    {
        OFF,
        RED,
        GREEN,
    };

    app::Timer toggle_timer(RED_TOGGLE_TIME_MS);
    app::Timer bootup_ignore_timer(BOOTUP_IGNORE_TIME_MS);
    TsimColor  curr_color = TsimColor::OFF;
    TsimColor  next_color = TsimColor::RED;
} // namespace

void init()
{
    io::tsim::set_off();
    curr_color = TsimColor::OFF;
    next_color = TsimColor::RED;
    toggle_timer.stop();
    bootup_ignore_timer.restart();
}

void tick()
{
    const bool ignore_fault_on_bootup = bootup_ignore_timer.updateAndGetState() == app::Timer::TimerState::RUNNING;
    const bool bms_ok                 = app::can_rx::BMS_BmsLatchOk_get();
    const bool imd_ok                 = app::can_rx::BMS_ImdLatchOk_get();
    const bool bspd_ok                = app::can_rx::BMS_BspdLatchOk_get();
    const bool no_fault               = (bms_ok && imd_ok && bspd_ok) || ignore_fault_on_bootup;

    if (no_fault)
    {
        io::tsim::set_green();
        curr_color = TsimColor::GREEN;
        toggle_timer.stop();
        return;
    }

    switch (toggle_timer.updateAndGetState())
    {
        case app::Timer::TimerState::IDLE:
            io::tsim::set_red();
            curr_color = TsimColor::RED;
            next_color = TsimColor::OFF;
            toggle_timer.restart();
            break;
        case app::Timer::TimerState::EXPIRED:
            if (curr_color == TsimColor::OFF && next_color == TsimColor::RED)
            {
                io::tsim::set_red();
                curr_color = TsimColor::RED;
                next_color = TsimColor::OFF;
                toggle_timer.restart();
            }
            else if (curr_color == TsimColor::RED && next_color == TsimColor::OFF)
            {
                io::tsim::set_off();
                curr_color = TsimColor::OFF;
                next_color = TsimColor::RED;
                toggle_timer.restart();
            }
            else
            {
                io::tsim::set_red();
                curr_color = TsimColor::RED;
                next_color = TsimColor::OFF;
                toggle_timer.restart();
            }
            break;
        case app::Timer::TimerState::RUNNING:
            break;
        default:
            break;
    }
}

} // namespace app::tsim
