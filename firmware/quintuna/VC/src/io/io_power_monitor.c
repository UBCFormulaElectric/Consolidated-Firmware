#include "io_power_monitor.h"
#include "hw_i2cs.h"

bool io_power_monitor_init()
{
    voltage_bus voltage_address = { .VBAT = 0x0B, .V_BOOST = 0x0C,
                                    .V_ACC = 0x0D, .V_EXT = 0x0E }; //is this an ok way of doing it?
    return hw_i2c_isTargetReady(&pwr_mon);
}

uint8_t io_power_monitor_read(voltage_bus voltage_address) // another way of doing it could be
                                                           // where I could get all 4 bus line voltages and store them in a struct
{
    //might change to uint8_t
    uint8_t buffer[1];
    hw_i2c_transmit(&pwr_mon, 0x1F, sizeof(0x1F)); // better buffer size - refresh_V
    hw_12c_transmit(&pwr_mon, voltage_address, sizeof(voltage_address)); // change register pointer address
    hw_i2c_receive(&pwr_mon, buffer, sizeof(buffer)); // get raw register data and store it in buffer
    return buffer[0];
}

// am I missing anything else for the power monitoring chip??
//app layer will have the processing of the raw voltage data
