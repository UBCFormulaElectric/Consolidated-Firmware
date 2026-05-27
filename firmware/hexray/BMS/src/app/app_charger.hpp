#pragma once

namespace app::charger
{

struct ElconRx
{
    bool  hardware_failure;
    bool  over_temperature;
    bool  input_voltage_fault;
    bool  charging_state_fault;
    bool  comm_timeout;
    float output_voltage_v;
    float output_current_a;
};

struct ElconTx
{
    float max_voltage_v; // physical units
    float max_current_a;
    bool  stop_charging; // true = send 0x01 in control byte
};

// Read current charger status from CAN→typed struct
ElconRx readElconStatus();

// Push a charging command to the BMS CAN TX table
void setTxFrame(const ElconTx &cmd);

// Get available charging current (A) computed from EVSE CP duty cycle.
// Returns 0.0f when the duty cycle is outside supported ranges.
float getAvailableCurrent();
} // namespace app::charger