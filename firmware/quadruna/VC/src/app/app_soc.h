#pragma once
#include "app_timer.h"

/*
    SOC algorithim:
    1) fetch the iniitial charge from a lookup table
    2) integrate current with respect to timef, to obtain the change in charge
    3) subtract the change in charge from our running total
    4) goto 2
*/

// 3500 mAh * 2 cells * 3.6C/mAh
#define MAX_CHARGE_C (3500.0f * 2.0f * 3.6f)

typedef struct
{
    // charge in cell in coulombs
    double charge_c;

    // Charge loss at time t-1
    float prev_current_A;

    TimerChannel soc_timer;
} SocStats;

// ocv (voltage) to soc lookup table
// used for initialziing soc before coulomb counting
/*
    Steps to initialize SOC from lut:
    1) Find the closest voltage to the sensor voltage in the table
    2) The index of the voltage in the table / SOC_OCV_LUT_SIZE = SOC
    3) SOC * MAX_CHARGE_C = initial charge
*/
// TODO: update ocv lut with charecterized values
#define OCV_SOC_LUT_SIZE (200)
static const float ocv_soc_lut[OCV_SOC_LUT_SIZE] = {
    3.0f,  3.0f,  3.01f, 3.01f, 3.02f, 3.03f, 3.03f, 3.04f, 3.04f, 3.05f, 3.06f, 3.06f, 3.07f, 3.07f, 3.08f, 3.09f,
    3.09f, 3.1f,  3.1f,  3.11f, 3.12f, 3.12f, 3.13f, 3.13f, 3.14f, 3.15f, 3.15f, 3.16f, 3.16f, 3.17f, 3.18f, 3.18f,
    3.19f, 3.19f, 3.2f,  3.21f, 3.21f, 3.22f, 3.22f, 3.23f, 3.24f, 3.24f, 3.25f, 3.25f, 3.26f, 3.27f, 3.27f, 3.28f,
    3.28f, 3.29f, 3.3f,  3.3f,  3.31f, 3.31f, 3.32f, 3.33f, 3.33f, 3.34f, 3.34f, 3.35f, 3.36f, 3.36f, 3.37f, 3.37f,
    3.38f, 3.39f, 3.39f, 3.4f,  3.4f,  3.41f, 3.42f, 3.42f, 3.43f, 3.43f, 3.44f, 3.45f, 3.45f, 3.46f, 3.46f, 3.47f,
    3.48f, 3.48f, 3.49f, 3.49f, 3.5f,  3.51f, 3.51f, 3.52f, 3.52f, 3.53f, 3.54f, 3.54f, 3.55f, 3.55f, 3.56f, 3.57f,
    3.57f, 3.58f, 3.58f, 3.59f, 3.6f,  3.6f,  3.61f, 3.61f, 3.62f, 3.63f, 3.63f, 3.64f, 3.64f, 3.65f, 3.66f, 3.66f,
    3.67f, 3.67f, 3.68f, 3.69f, 3.69f, 3.7f,  3.7f,  3.71f, 3.72f, 3.72f, 3.73f, 3.73f, 3.74f, 3.75f, 3.75f, 3.76f,
    3.76f, 3.77f, 3.78f, 3.78f, 3.79f, 3.79f, 3.8f,  3.81f, 3.81f, 3.82f, 3.82f, 3.83f, 3.84f, 3.84f, 3.85f, 3.85f,
    3.86f, 3.87f, 3.87f, 3.88f, 3.88f, 3.89f, 3.9f,  3.9f,  3.91f, 3.91f, 3.92f, 3.93f, 3.93f, 3.94f, 3.94f, 3.95f,
    3.96f, 3.96f, 3.97f, 3.97f, 3.98f, 3.99f, 3.99f, 4.0f,  4.0f,  4.01f, 4.02f, 4.02f, 4.03f, 4.03f, 4.04f, 4.05f,
    4.05f, 4.06f, 4.06f, 4.07f, 4.08f, 4.08f, 4.09f, 4.09f, 4.1f,  4.11f, 4.11f, 4.12f, 4.12f, 4.13f, 4.14f, 4.14f,
    4.15f, 4.15f, 4.16f, 4.17f, 4.17f, 4.18f, 4.18f, 4.19f
};

void  app_soc_init(void);
void  app_soc_update(void);
float app_soc_getPercent(void);
void  app_soc_broadcast(void);
float app_soc_getChargeFromOcv(void);