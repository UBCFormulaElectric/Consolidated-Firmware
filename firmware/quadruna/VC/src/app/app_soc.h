#pragma once
#include "app_timer.h"

/*
    SOC algorithim (coulomb counting):
    1) fetch the iniitial charge from a lookup table
    2) integrate current with respect to timef, to obtain the change in charge
    3) subtract the change in charge from our running total
    4) goto 2
*/

#define SOC_TIMER_DURATION_MS (110U)
#define MS_TO_S (0.001)

// 2600 mAh * 2 cells * 3.6C/mAh
#define MAX_CHARGE_C (2600.0f * 2.0f * 3.6f)

typedef struct
{
    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    TimerChannel soc_timer;
} SocStats;

// known (ocv, coulumbs) points on the discharge curve
// used to initialize soc before coulomb counting via linear spline
// obtained from Discharge Rate Charecteristics graph @
// https://www.molicel.com/wp-content/uploads/INR18650P26A-V2-80087.pdf ordered from min to max voltage
#define OCV_CHARGE_SPLINE_ANCHORS_SIZE (9)
static const float ocv_charge_spline_anchors[OCV_CHARGE_SPLINE_ANCHORS_SIZE][2] = {
    { 2.49f, 158.4f }, { 2.9994f, 679.7f }, { 3.178f, 1180.8f }, { 3.275f, 1800.0f },  { 3.403f, 3333.6f },
    { 3.5f, 5162.4f }, { 3.66f, 10152.0f }, { 4.09f, 17870.4f }, { 4.1674f, 18702.0f }
};

void  app_soc_init(void);
void  app_soc_update(void);
float app_soc_getPercent(void);
void  app_soc_broadcast(void);
float app_soc_getChargeFromOcv(void);