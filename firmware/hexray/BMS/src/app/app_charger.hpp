#pragma once

namespace app::charger
{

struct ElconFaultConfig
{
    bool hardware_failure;
    bool over_temperature;
    bool input_voltage_fault;
    bool charging_state_fault;
    bool comm_timeout;

    operator bool() const
    {
        return hardware_failure || over_temperature || input_voltage_fault || charging_state_fault || comm_timeout;
    }
};

struct ElconChargingConfig
{
    float max_voltage_v; // physical units
    float max_current_a;
    bool  stop_charging; // true = send 0x01 in control byte
};

// Read current charger status from CAN→typed struct
ElconFaultConfig getFaultStatus();

// Push a charging command to the BMS CAN TX table
void setChargingConfig(const ElconChargingConfig &cmd);

// Get available charging current (A) computed from EVSE CP duty cycle.
// Returns 0.0f when the duty cycle is outside supported ranges.
float getAvailableCurrent();

// Get the output current of the charger in A, as reported by the Elcon on CAN.
float getOutputCurrent();

// Broadcast charger-related CAN signals that are updated at 100Hz (EVSE CP duty cycle + frequency, available current)
void broadcast();
} // namespace app::charger