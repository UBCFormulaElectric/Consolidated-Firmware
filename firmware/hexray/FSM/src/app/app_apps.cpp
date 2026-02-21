#include "app_apps.hpp"
#include "app_signal.hpp"

#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include "io_apps.hpp"
#include <cmath>

namespace app::apps
{
static constexpr uint32_t DISAGREEMENT_TIME_TO_FAULT = 10;
static constexpr uint32_t DISAGREEMENT_TIME_TO_CLEAR = 10;
static constexpr uint32_t OCSC_TIME_TO_FAULT         = 10;
static constexpr uint32_t OCSC_TIME_TO_CLEAR         = 10;

static Signal papps_ocsc_signal(OCSC_TIME_TO_FAULT, OCSC_TIME_TO_CLEAR);
static Signal sapps_ocsc_signal(OCSC_TIME_TO_FAULT, OCSC_TIME_TO_CLEAR);
static Signal papps_sapps_disagreement_signal(DISAGREEMENT_TIME_TO_FAULT, DISAGREEMENT_TIME_TO_CLEAR);

void init()
{
    io::apps::init();
}

void broadcast()
{
    const float papps_pedal_percentage = io::apps::getPrimary();
    const float sapps_pedal_percentage = io::apps::getSecondary();

    // OCSC test (primary)
    const bool                primary_pedal_ocsc = io::apps::isPrimaryOCSC();
    const Signal::SignalState papps_ocsc_signal_state =
        papps_ocsc_signal.get_updated_state(primary_pedal_ocsc, !primary_pedal_ocsc);
    const bool papps_ocsc_active = papps_ocsc_signal_state == Signal::SignalState::ACTIVE;

    // OCSC test (secondary)
    const bool                secondary_pedal_ocsc = io::apps::isSecondaryOCSC();
    const Signal::SignalState sapps_ocsc_signal_state =
        sapps_ocsc_signal.get_updated_state(secondary_pedal_ocsc, !secondary_pedal_ocsc);
    const bool sapps_ocsc_active = sapps_ocsc_signal_state == Signal::SignalState::ACTIVE;

    can_alerts::warnings::PappsOCSC_set(papps_ocsc_active);
    can_alerts::warnings::SappsOCSC_set(sapps_ocsc_active);

    const float               papps_sapps_diff = fabsf(papps_pedal_percentage - sapps_pedal_percentage);
    const Signal::SignalState papps_sapps_disagreement_signal_state =
        papps_sapps_disagreement_signal.get_updated_state((papps_sapps_diff) > 10.f, (papps_sapps_diff) <= 10.f);
    const bool papps_sapps_disagreement_active = papps_sapps_disagreement_signal_state == Signal::SignalState::ACTIVE;

    can_alerts::warnings::AppsDisagreement_set(papps_sapps_disagreement_active);

    can_tx::FSM_PappsRawPedalPercentage_set(papps_pedal_percentage);
    can_tx::FSM_SappsRawPedalPercentage_set(sapps_pedal_percentage);

    // set mapped apps to 0 if anything went wrong
    if (papps_ocsc_active || sapps_ocsc_active || papps_sapps_disagreement_active)
    {
        can_tx::FSM_PappsMappedPedalPercentage_set(0.0f);
        can_tx::FSM_SappsMappedPedalPercentage_set(0.0f);
    }
    else
    {
        can_tx::FSM_PappsMappedPedalPercentage_set(papps_pedal_percentage);
        can_tx::FSM_SappsMappedPedalPercentage_set(sapps_pedal_percentage);
    }
}
} // namespace app::apps
