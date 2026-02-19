#include "app_screens.hpp"
#include "io_rotary.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

namespace app::screens
{
/************************* Screens ***************************/
// extern Screen main_drive_screen;
// extern Screen drive_modes_screen;
// extern Screen start_up_screen;
// extern Screen init_screen;
/************************* Global Variables ***************************/
inline constexpr size_t NUM_DEVICE_SCREENS = 2u;
static uint8_t          current_screen     = 0;
static Screen           drive_screens[NUM_DEVICE_SCREENS];

/*********************** Function Definitions ***************************/
/**
 * @brief Initalize all screens, and starting init screen.
 */
void init()
{
    io::rotary::init();
    io::rotary::setClockwiseCallback([] { drive_screens[current_screen].cw_callback(); });
    io::rotary::setCounterClockwiseCallback([] { drive_screens[current_screen].ccw_callback(); });
    io::rotary::setPushCallback(
        []
        {
            // Only multiple screens in drive state.
            if (const can_utils::VCState vc_state = can_rx::VC_State_get();
                vc_state != can_utils::VCState::VC_DRIVE_STATE)
            {
                return;
            }
            current_screen = static_cast<uint8_t>((current_screen + 1) % NUM_DEVICE_SCREENS);
        });

    // drive_screens[0] = main_drive_screen;
    // drive_screens[1] = drive_modes_screen;
}

/**
 * @brief Called every 100Hz, updates seven seg.
 */
void tick()
{
    // Display screen, based on VC state.
    if (const can_utils::VCState vc_state = can_rx::VC_State_get(); vc_state == can_utils::VCState::VC_INIT_STATE)
    {
        // init_screen.update();
    }
    else if (vc_state == can_utils::VCState::VC_DRIVE_STATE || vc_state == can_utils::VCState::VC_DRIVE_WARNING_STATE)
    {
        drive_screens[current_screen].update();
    }
    else
    {
        // start_up_screen.update();
    }
}
} // namespace app::screens