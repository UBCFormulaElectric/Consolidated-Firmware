#pragma once

typedef struct
{
    double Fz_fr;
    double Fz_fl;
    double Fz_rr;
    double Fz_rl;
} VD_State;

extern VD_State state;

void app_stateEstimation_estimate(void);