#pragma once

#define EFFICIENCY_LUT_RPM_INTERVAL (500U)
#define EFFICIENCY_LUT_NUM_ENTRIES (14U)

struct EfficiencyLUTEntry
{
    float speed_rpm;
    float efficiency_estimate;
};

struct EfficiencyLUTEntry EfficiencyLUT[EFFICIENCY_LUT_NUM_ENTRIES] = {
    [0]  = { .speed_rpm = 500.0f, .efficiency_estimate = 0.80f },
    [1]  = { .speed_rpm = 1000.0f, .efficiency_estimate = 0.80f },
    [2]  = { .speed_rpm = 1500.0f, .efficiency_estimate = 0.80f },
    [3]  = { .speed_rpm = 2000.0f, .efficiency_estimate = 0.80f },
    [4]  = { .speed_rpm = 2500.0f, .efficiency_estimate = 0.80f },
    [5]  = { .speed_rpm = 3000.0f, .efficiency_estimate = 0.80f },
    [6]  = { .speed_rpm = 3500.0f, .efficiency_estimate = 0.80f },
    [7]  = { .speed_rpm = 4000.0f, .efficiency_estimate = 0.80f },
    [8]  = { .speed_rpm = 4500.0f, .efficiency_estimate = 0.80f },
    [9]  = { .speed_rpm = 5000.0f, .efficiency_estimate = 0.80f },
    [10] = { .speed_rpm = 5500.0f, .efficiency_estimate = 0.80f },
    [11] = { .speed_rpm = 6000.0f, .efficiency_estimate = 0.80f },
    [12] = { .speed_rpm = 6500.0f, .efficiency_estimate = 0.80f },
    [13] = { .speed_rpm = 7000.0f, .efficiency_estimate = 0.80f } // Maximum motor speed = 7000 rpm
};
