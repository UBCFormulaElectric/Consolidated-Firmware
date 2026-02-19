#include "app_screens.hpp"

#include "io_rotary.hpp"
#include "app_canRx.hpp"
#include <functional>

namespace app::screens
{
/************************* Global Variables ***************************/
inline constexpr size_t NUM_DEVICE_SCREENS = 2u;
inline constexpr size_t LV_MAX             = 2u;
inline constexpr size_t HV_MAX             = 2u;
static size_t           current_screen     = 0;

static std::array<const Screen *, NUM_DEVICE_SCREENS> drive_screens = { {
    &shdn_screen,
    &brightness_screen,
} };

/*********************** Function Definitions ***************************/
void init()
{
    io::rotary::init();
    io::rotary::setClockwiseCallback([] { drive_screens[current_screen]->cw_callback(); });
    io::rotary::setCounterClockwiseCallback([] { drive_screens[current_screen]->ccw_callback(); });
    io::rotary::setPushCallback(
        []
        {
            // transition to next
            size_t max_screens;
            switch (can_rx::VC_State_get())
            {
                case can_utils::VCState::VC_DRIVE_WARNING_STATE:
                case can_utils::VCState::VC_FAULT_STATE:
                    // disable rotary
                    return;
                case can_utils::VCState::VC_PCM_ON_STATE:
                case can_utils::VCState::VC_HV_INIT_STATE:
                case can_utils::VCState::VC_INVERTER_ON_STATE:
                case can_utils::VCState::VC_BMS_ON_STATE:
                case can_utils::VCState::VC_INIT_STATE:
                default:
                    max_screens = LV_MAX;
                    break;
                case can_utils::VCState::VC_HV_ON_STATE:
                case can_utils::VCState::VC_DRIVE_STATE:
                    max_screens = HV_MAX;
                    break;
            }
            current_screen = (current_screen + 1) % max_screens;
        });

    // drive_screens[0] = main_drive_screen;
    // drive_screens[1] = drive_modes_screen;
}

void tick()
{
    // force transitions based on vc state
    switch (can_rx::VC_State_get())
    {
        case can_utils::VCState::VC_DRIVE_WARNING_STATE:
        case can_utils::VCState::VC_FAULT_STATE:
            alerts_screen.update();
            return;
        case can_utils::VCState::VC_INIT_STATE:
        case can_utils::VCState::VC_INVERTER_ON_STATE:
        case can_utils::VCState::VC_PCM_ON_STATE:
        case can_utils::VCState::VC_HV_INIT_STATE:
        case can_utils::VCState::VC_HV_ON_STATE:
        case can_utils::VCState::VC_BMS_ON_STATE:
        case can_utils::VCState::VC_DRIVE_STATE:
        default:
            break;
    }
    drive_screens[current_screen]->update();
}
} // namespace app::screens