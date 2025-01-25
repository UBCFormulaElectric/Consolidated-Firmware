#include "io_power_monitor.h"
#include "hw_i2cs.h"

#define VOLTAGE_BUS_TRANSFER_FACTOR 4.8828e-4f;
#define VOLTAGE_SENSE_TRANSFER_FACTOR 1.5259e-6f;
#define SENSE_RESISTOR 2.0e6f;
#define FSC 5.00e-8f // Full-Scale Current

// const power_rail bat_bus{
//     .address = 0x07 // VBUS1 got removed?
// };
const power_rail boost = {
    .address = 0x08 // VBUS2
};
const power_rail acc = {
    .address = 0x09 // VBUS3
};
const power_rail ext = {
    .address = 0x0A // VBUS4
};

bool io_power_monitor_init()
{
    return hw_i2c_isTargetReady(&pwr_mon);
}

float io_power_monitor_read_voltage(const power_rail *voltage_address)
{
    uint8_t buffer[1];

    buffer[0] = 0x1F;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // refresh

    buffer[0] = voltage_address->address;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon, buffer, 1); // get raw register data and store it in buffer

    const float voltage_buffer = (float)buffer[0] * VOLTAGE_BUS_TRANSFER_FACTOR;

    return voltage_buffer;
}

float io_power_monitor_read_current(const power_rail *current_address)
{
    // This repeat gets the Vsense value
    uint8_t buffer[1];

    buffer[0] = 0x1F;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // refresh

    buffer[0] = (current_address->address) + 0x04;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address (Vsense)

    hw_i2c_receive(&pwr_mon, buffer, 1); // get raw register data and store it in buffer

    const float voltage_sense_buffer = (float)buffer[0] * VOLTAGE_SENSE_TRANSFER_FACTOR;

    return FSC * (voltage_sense_buffer / 65536.0f); // FSC * (Vsense/denom)
}

// check alert bit function
