#include "app_bspdwarning.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"

namespace app::bspdWarning
{
static constexpr uint32_t app_brake_diagreement_time_to_fault = 10u;
static constexpr uint32_t app_brake_diagreement_time_to_clear = 10u;

static Signal apps_brake_disagreement_state{ app_brake_diagreement_time_to_fault, app_brake_diagreement_time_to_clear };

bool checkSoftwareBspd(const float papps_pedal_percentage)
{
    const bool apps_brakes_conflict     = can_rx::FSM_BrakeActuated_get() && (papps_pedal_percentage > 25.0f);
    const bool apps_less_than_5_percent = (papps_pedal_percentage < 5.0f);

    const bool apps_brake_disagreement_active =
        (apps_brake_disagreement_state.get_updated_state(apps_brakes_conflict, apps_less_than_5_percent) ==
         Signal::SignalState::ACTIVE);

    can_tx::VC_Warning_SoftwareBspd_set(apps_brake_disagreement_active);

    return apps_brake_disagreement_active;
}

#ifdef TARGET_TEST
void reset()
{
    apps_brake_disagreement_state.reset();
    app::can_tx::VC_Warning_SoftwareBspd_set(false);
}
#endif

} // namespace app::bspdWarning
