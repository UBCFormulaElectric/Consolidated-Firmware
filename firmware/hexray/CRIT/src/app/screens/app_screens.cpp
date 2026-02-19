#include "app_screens.hpp"

#include "io_rotary.hpp"
#include "app_canRx.hpp"

namespace app::screens
{
/************************* Screens ***************************/
// extern Screen main_drive_screen;
// extern Screen drive_modes_screen;
// extern Screen start_up_screen;
extern Screen init_screen;
/************************* Global Variables ***************************/
// inline constexpr size_t NUM_DEVICE_SCREENS = 2u;
// static uint8_t          current_screen     = 0;
// static Screen           drive_screens[NUM_DEVICE_SCREENS];
static Screen &current_screen = init_screen;

/*********************** Function Definitions ***************************/
void init()
{
    io::rotary::init();
    io::rotary::setClockwiseCallback([] { current_screen.cw_callback(); });
    io::rotary::setCounterClockwiseCallback([] { current_screen.ccw_callback(); });
    io::rotary::setPushCallback(
        []
        {
            // Only multiple screens in drive state.
            if (const can_utils::VCState vc_state = can_rx::VC_State_get();
                vc_state != can_utils::VCState::VC_DRIVE_STATE)
            {
                return;
            }
            // transition to next
        });

    // drive_screens[0] = main_drive_screen;
    // drive_screens[1] = drive_modes_screen;
}

void tick()
{
    // force transitions based on vc state
    switch (can_rx::VC_State_get())
    {
        case can_utils::VCState::VC_INIT_STATE:
            // init_screen.update();
            break;
        case can_utils::VCState::VC_DRIVE_WARNING_STATE:
        case can_utils::VCState::VC_DRIVE_STATE:
            // drive_screens[current_screen].update();
            break;
        case can_utils::VCState::VC_FAULT_STATE:
        case can_utils::VCState::VC_INVERTER_ON_STATE:
        case can_utils::VCState::VC_PCM_ON_STATE:
        case can_utils::VCState::VC_HV_INIT_STATE:
        case can_utils::VCState::VC_HV_ON_STATE:
        case can_utils::VCState::VC_BMS_ON_STATE:
        default:
            // start_up_screen.update();
            break;
    }
    current_screen.update();
}
} // namespace app::screens