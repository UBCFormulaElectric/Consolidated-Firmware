#pragma once

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_chargeState_get(void);

typedef struct {
    bool hardwareFailure;
    bool overTemperature;
    bool inputVoltageFault;
    bool chargingStateFault;
    bool commTimeout;
    float outputVoltage_V;
    float outputCurrent_A;
} ElconRx;

typedef struct {
    float maxVoltage_V;   // physical units
    float maxCurrent_A;
    bool  stopCharging;   // true = send 0x01 in control byte 
} ElconTx;

/**
 * @return Elcon status
 */
static ElconRx readElconStatus(void);