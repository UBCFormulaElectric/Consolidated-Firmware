#include "app_soc.h"
#include "io_currentSensing.h"
#include "app_math.h"
#include "app_canTx.h"
#include "io_lowVoltageBattery.h"

static SocStats stats;

void app_soc_init(void)
{
    stats.prev_current_A = 0.0f;

    stats.charge_c = app_soc_getChargeFromOcv();
    // TODO: initialize soc from SD card, not ocv

    app_timer_init(&stats.soc_timer, SOC_TIMER_DURATION_MS);
}

void app_soc_update(void)
{
    // NOTE current sign is relative to current into the battery
    double *charge_c     = &stats.charge_c;
    float  *prev_current = &stats.prev_current_A;
    float   current      = io_currentSensing_getBatteryCurrent();

    double elapsed_time_s = (double)app_timer_getElapsedTime(&stats.soc_timer) * MS_TO_S;
    app_timer_restart(&stats.soc_timer);

    // Trapezoidal Rule adds integral of current time-step to previous integral value.
    app_math_trapezoidalRule(charge_c, prev_current, current, elapsed_time_s);
}

float app_soc_getPercent()
{
    return (float)stats.charge_c / MAX_CHARGE_C * 100.0f;
}

void app_soc_broadcast()
{
    app_canTx_VC_Soc_set(app_soc_getPercent());
}

float app_soc_getChargeFromOcv(void)
{
    float battery_ocv = io_lowVoltageBattery_getBatVoltage();

    // identify between which two anchor points our battery voltage lies
    int   i          = 0;
    float ocv_anchor = ocv_charge_spline_anchors[i][0];

    while (ocv_anchor < battery_ocv)
    {
        i += 1;
        ocv_anchor = ocv_charge_spline_anchors[i][0];
    }

    int left_anchor_index  = i - 1;
    int right_anchor_index = i;

    // if voltage lower than minimum anchor, assume data extends first sector
    if (left_anchor_index == -1)
    {
        left_anchor_index  = 0;
        right_anchor_index = 1;
    }

    // if voltage higher than maximum anchor, assume data extends last sector
    if (right_anchor_index == OCV_CHARGE_SPLINE_ANCHORS_SIZE)
    {
        left_anchor_index  = OCV_CHARGE_SPLINE_ANCHORS_SIZE - 2;
        right_anchor_index = OCV_CHARGE_SPLINE_ANCHORS_SIZE - 1;
    }

    float left_ocv_anchor     = ocv_charge_spline_anchors[left_anchor_index][0];
    float left_charge_anchor  = ocv_charge_spline_anchors[left_anchor_index][1];
    float right_ocv_anchor    = ocv_charge_spline_anchors[right_anchor_index][0];
    float right_charge_anchor = ocv_charge_spline_anchors[right_anchor_index][1];

    // compute slope in sector
    float charge_per_voltage = (right_charge_anchor - left_charge_anchor) / (right_ocv_anchor - left_ocv_anchor);

    // extrapolate charge
    // y = m(x - x_0) + y_0
    return (battery_ocv - left_ocv_anchor) * charge_per_voltage + left_charge_anchor;
}