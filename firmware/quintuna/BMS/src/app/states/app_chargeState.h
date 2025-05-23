#pragma once
#include <stdbool.h>

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_chargeState_get(void);

typedef struct
{
    bool  hardwareFailure;
    bool  overTemperature;
    bool  inputVoltageFault;
    bool  chargingStateFault;
    bool  commTimeout;
    float outputVoltage_V;
    float outputCurrent_A;
} ElconRx;

typedef struct
{
    float maxVoltage_V; // physical units
    float maxCurrent_A;
    bool  stopCharging; // true = send 0x01 in control byte
} ElconTx;

// Structure to hold min/max DC current based on AC supply extremes
typedef struct
{
    float idc_min; // A – DC current at VAC_MIN
    float idc_max; // A – DC current at VAC_MAX
} DCRange_t;

/**
 * @return Elcon status
 */
ElconRx readElconStatus(void);