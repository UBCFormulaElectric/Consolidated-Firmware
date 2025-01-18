#include "io_power_monitor.h"
#include "hw_i2cs.h"

#define VOLTAGE_TRANSFER_FACTOR 4.8e-4f;

const power_rail bat_bus {
    .address = 0x07 // VBUS1
};
const power_rail boost_bus = {
    .address = 0x08 // VBUS2
};
const power_rail acc_bus = {
    .address = 0x09 // VBUS3
};
const power_rail ext_bus = {
    .address = 0x0A // VBUS4
};

bool io_power_monitor_init()
{
    return hw_i2c_isTargetReady(&pwr_mon);
}

float io_power_monitor_read_voltage(const power_rail* voltage_address)
{
    uint16_t buffer[1];

    buffer[0] = 0x1F;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // refresh

    buffer[0] = voltage_address->address;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon, buffer, 1);  // get raw register data and store it in buffer

    const float voltage_buffer = (float) buffer[0] * VOLTAGE_TRANSFER_FACTOR;

    return voltage_buffer;
}
