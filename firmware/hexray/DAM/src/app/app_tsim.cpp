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

    static void apply_tsim_color(const TsimColor color)
    {
        switch (color)
        {
            case TsimColor::OFF:
                io::tsim::set_off();
                break;
            case TsimColor::RED:
                io::tsim::set_red();
                break;
            case TsimColor::GREEN:
                io::tsim::set_green();
                break;
            default:
                io::tsim::set_off();
                break;
        }
    }

    static void apply_toggle_transition(const TsimColor output_color, const TsimColor next_after_output)
    {
        apply_tsim_color(output_color);
        curr_color = output_color;
        next_color = next_after_output;
        toggle_timer.restart();
    }
} // namespace

void init()
{
    apply_tsim_color(TsimColor::OFF);
    curr_color = TsimColor::OFF;
    next_color = TsimColor::RED;
    toggle_timer.stop();
    bootup_ignore_timer.restart();
}

void tick()
{
    const bool ignore_fault_on_bootup = bootup_ignore_timer.updateAndGetState() == app::Timer::TimerState::RUNNING;
    const bool no_fault               = (app::can_rx::BMS_BmsLatchOk_get() && app::can_rx::BMS_ImdLatchOk_get() &&
                           app::can_rx::BMS_BspdLatchOk_get()) ||
                          ignore_fault_on_bootup;

    if (no_fault)
    {
        apply_tsim_color(TsimColor::GREEN);
        curr_color = TsimColor::GREEN;
        toggle_timer.stop();
        return;
    }

    switch (toggle_timer.updateAndGetState())
    {
        case app::Timer::TimerState::IDLE:
            apply_toggle_transition(TsimColor::RED, TsimColor::OFF);
            break;
        case app::Timer::TimerState::EXPIRED:
            if (curr_color == TsimColor::OFF && next_color == TsimColor::RED)
            {
                apply_toggle_transition(TsimColor::RED, TsimColor::OFF);
            }
            else if (curr_color == TsimColor::RED && next_color == TsimColor::OFF)
            {
                apply_toggle_transition(TsimColor::OFF, TsimColor::RED);
            }
            else
            {
                apply_toggle_transition(TsimColor::RED, TsimColor::OFF);
            }
            break;
        case app::Timer::TimerState::RUNNING:
            break;
        default:
            break;
    }
}

} // namespace app::tsim
