#pragma once

namespace app::charger
{
    
struct ElconFaultStatus
{
    bool hardware_failure;
    bool over_temperature;
    bool input_voltage_fault;
    bool charging_disabled;
    bool comm_timeout;

    explicit operator bool() const       // Allows easy checking to see if any fault is present, prevents implicit conversion
    {
        return hardware_failure || over_temperature || input_voltage_fault || charging_disabled || comm_timeout;
    }

};

struct ElconChargingConfig
{
    float max_voltage_v; // physical units
    float max_current_a;
    bool  stop_charging; // true = send 0x01 in control byte
};

// Returns the output current in A as reported by the Elcon charger on CAN. 
float getOutputCurrent();

// Reads current charger status from CAN and returns it in a typed struct
ElconFaultStatus getFaultStatus();

// Sends a charging command to the BMS CAN TX table, which will be sent to the Elcon charger on the next CAN TX cycle
void setChargingConfig(const ElconChargingConfig &cmd);

// Returns the available charging current in A, computed from the EVSE CP duty cycle. Returns 0.0f when the duty cycle is outside supported ranges.
float getAvailableCurrent(); 

// Broadcasts charger-related CAN signals (EVSE CP duty cycle + frequency, available current) to the BMS CAN TX table
void broadcastEvseStatus();

} // namespace app::charger