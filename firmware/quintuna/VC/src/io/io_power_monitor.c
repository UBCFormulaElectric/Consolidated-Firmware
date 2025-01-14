#include "io_power_monitor.h"
#include "hw_i2cs.h"

const power_rail bat {
    .address = 0x0A
};
const power_rail boost = {
    .address = 0x0A
};
const power_rail acc = {
    .address = 0x0A
};
const power_rail ext = {
    .address = 0x0A
};

bool io_power_monitor_init()
{
    return hw_i2c_isTargetReady(&pwr_mon);
}

uint8_t io_power_monitor_read_voltage(const power_rail* voltage_address)
{
    uint8_t buffer[1];

    buffer[0] = 0x1F;
    hw_i2c_transmit(&pwr_mon, buffer, 1); //

    buffer[0] = voltage_address->address;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon, buffer, 1);  // get raw register data and store it in buffer

    //transfer functions here

    return buffer[0];
}
