#include "app_bspdwarning.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"

namespace app::bspdWarning
{

// TODO: integrate with warnings (will have to make compatible with regen)
bool checkSoftwareBspd(float papps_pedal_percentage, app::Signal signal)
{
    const bool brake_actuation_state    = app::can_rx::FSM_BrakeActuated_get();
    const bool apps_brakes_conflict     = brake_actuation_state && (papps_pedal_percentage > 0.25f);
    const bool apps_less_than_5_percent = (papps_pedal_percentage < 0.05f);

    const bool apps_brake_disagreement_active =
        (signal.get_updated_state(apps_brakes_conflict, apps_less_than_5_percent) == app::Signal::SignalState::ACTIVE);

    app::can_tx::VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

} // namespace app::bspdWarning