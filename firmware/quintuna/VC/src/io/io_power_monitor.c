#include "io_power_monitor.h"
#include "hw_i2cs.h"

#define VOLTAGE_BUS_TRANSFER_FACTOR 4.8828e-4f;
#define VOLTAGE_SENSE_TRANSFER_FACTOR 1.5259e-6f;
#define SENSE_RESISTOR 2.0e6f;
#define FSC 5.00e-8f // Full-Scale Current
#define REFRESH 0x1F
#define ALERT_STATUS 0x26

const PowerRail boost = {.address = 0x08}; // VBUS2
const PowerRail acc = {.address = 0x09}; // VBUS3
const PowerRail ext = {.address = 0x0A}; // VBUS4

bool io_power_monitor_init()
{
    return hw_i2c_isTargetReady(&pwr_mon);
}

float io_power_monitor_read_voltage(const PowerRail *voltage_address)
{
    uint8_t buffer[1];

    buffer[0] = REFRESH;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // refresh

    buffer[0] = voltage_address->address;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon, buffer, 1); // get raw register data and store it in buffer

    const float voltage_buffer = (float)buffer[0] * VOLTAGE_BUS_TRANSFER_FACTOR;

    return voltage_buffer;
}

float io_power_monitor_read_current(const PowerRail *current_address)
{
    uint8_t buffer[1];

    buffer[0] = REFRESH;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // refresh

    buffer[0] = (current_address->address) + 0x04;
    hw_i2c_transmit(&pwr_mon, buffer, 1); // change register pointer address (Vsense)

    hw_i2c_receive(&pwr_mon, buffer, 1); // get raw register data and store it in buffer

    const float voltage_sense_buffer = (float)buffer[0] * VOLTAGE_SENSE_TRANSFER_FACTOR;

    return FSC * (voltage_sense_buffer / 65536.0f); // FSC * (Vsense/denom)
}

uint32_t io_power_monitor_alert_status()
{
    uint8_t buffer[3]; // Does not require a REFRESH

    buffer[0] = 0x26; // ALERT_STATUS address
    hw_i2c_transmit(&pwr_mon, buffer, 1);

    hw_i2c_receive(&pwr_mon, buffer, 3); // Gets all the alert statuses

    const uint32_t alert_buffer =
        (uint32_t)((buffer[0] << 16) | (buffer[1] << 8) | buffer[2]); // 23:0 bits for alert (datasheet)

    return alert_buffer;
}
