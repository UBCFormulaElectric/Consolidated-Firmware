#include "app_warningHandling.hpp"
extern "C"
{
#include "app_canAlerts.h"
#include "app_canRx.h"
#include "app_canTx.h"
}

namespace app::warningHandling
{

app::Signal softwareBspd_init(void)
{
    app::Signal apps_brake_disagreement_state{app_brake_diagreement_time_to_fault, app_brake_diagreement_time_to_clear};
}

bool boardWarningCheck(void)
{
    return app::can::alerts::AnyBoardHasWarning();
}

// TODO: integrate with warnings (will have to make compatible with regen)
bool checkSoftwareBspd(float papps_pedal_percentage, app::Signal signal)
{
    const bool brake_actuation_state = app::can::rx::FSM_BrakeActuated_get();
    const bool apps_brakes_conflict  = brake_actuation_state && (papps_pedal_percentage > 0.25f);
    const bool apps_less_than_5_percent = (papps_pedal_percentage < 0.05f);

    const bool apps_brake_disagreement_active = (signal::get_updated_state(apps_brakes_conflict, apps_less_than_5_percent) == SIGNAL_STATE_ACTIVE);

    app::can::alerts::VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

} // namespace app::warningHandling