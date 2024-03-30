#include "app_soc.h"
#include "io_currentSensing.h"
#include "app_math.h"
#include "app_canTx.h"

static SocStats stats;

#define SOC_TIMER_DURATION_MS (110U)
#define MS_TO_S (0.001)

void app_soc_init(void) 
{
    stats.prev_current_A = 0.0f;
    stats.charge_c = 0; // TODO: init from ocv lookup

    app_timer_init(&stats.soc_timer, SOC_TIMER_DURATION_MS);
}

void app_soc_update(void)
{
    // NOTE current sign is relative to current into the battery
    double *charge_c     = &stats.charge_c;
    float  *prev_current = &stats.prev_current_A;
    float   current      = io_currentSensing_getBatteryCurrent();

    double elapsed_time_s = (double) app_timer_getElapsedTime(&stats.soc_timer) * MS_TO_S;
    app_timer_restart(&stats.soc_timer);

    // Trapezoidal Rule adds integral of current time-step to previous integral value.
    app_math_trapezoidalRule(charge_c, prev_current, current, elapsed_time_s);
}

float app_soc_getPercent() {
    return (float) (stats.charge_c / MAX_CHARGE_C * 100.0);
}

void app_soc_broadcast() {
    app_canTx_VC_Soc_set(app_soc_getPercent());
}