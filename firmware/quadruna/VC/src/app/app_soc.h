#pragma once
#include "app_timer.h"

/*
SOC algorithim:
1) fetch the iniitial charge from a lookup table
2) integrate current with respect to time, to obtain the change in charge
3) subtract the change in charge from our running total
4) goto 2
*/

#define MAX_CHARGE_C (2000.0)

typedef struct
{
    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    TimerChannel soc_timer;
} SocStats;

void app_soc_init(void);
void app_soc_update(void);
float app_soc_getPercent(void);
void app_soc_broadcast(void);