#include "io_power_monitor.h"
#include "hw_i2cs.h"

/*
 * For more info about this chip, the data sheet is linked below:
 * https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/PAC195X-Family-Data-Sheet-DS20006539.pdf
 *
 * A confluence Page will also be made for how the driver code goes about
 * using the PAC1954-E/4MX power monitoring chip
 *
 */

const PowerRail ext = { .bus_address = 0x07, .sense_address = 0x0B, .power_address = 0x17 };
const PowerRail acc = { .bus_address = 0x08, .sense_address = 0x0C, .power_address = 0x18 };

bool io_power_monitor_init()
{
    // Configuration of the ctrl register may be needed
    return hw_i2c_isTargetReady(&pwr_mon_i2c);
}

#define REFRESH 0x1F

void io_power_monitor_refresh()
{
    uint8_t buffer[1] = { REFRESH };
    hw_i2c_transmit(&pwr_mon_i2c, buffer, 1);
}

#define VOLTAGE_BUS_TRANSFER_FACTOR 4.8828e-4f;

float io_power_monitor_read_voltage(const PowerRail *voltage_address)
{
    uint8_t buffer[2];

    io_power_monitor_refresh(); // timing problems potentially?

    buffer[0] = voltage_address->bus_address;
    hw_i2c_transmit(&pwr_mon_i2c, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon_i2c, buffer, 2); // get raw register data and store it in buffer

    const float voltage_buffer = (float)((buffer[0] << 8) | buffer[1]) * VOLTAGE_BUS_TRANSFER_FACTOR;

    return voltage_buffer;
}

#define VOLTAGE_SENSE_TRANSFER_FACTOR 1.5259e-6f;
#define SENSE_RESISTOR 3.0e6f;

float io_power_monitor_read_current(const PowerRail *current_address)
{
    uint8_t buffer[2];

    io_power_monitor_refresh();

    buffer[0] = current_address->sense_address;
    hw_i2c_transmit(&pwr_mon_i2c, buffer, 1); // change register pointer address (Vsense)

    hw_i2c_receive(&pwr_mon_i2c, buffer, 2); // get raw register data and store it in buffer

    const float voltage_sense_buffer = (float)((buffer[0] << 8) | buffer[1]) * VOLTAGE_SENSE_TRANSFER_FACTOR;

    return voltage_sense_buffer / SENSE_RESISTOR; // FSC * (Vsense/denom)
}

#define VOLTAGE_POWER_TRANSFER_FACTOR 9.3132e-10f;
#define POWER_FSR 1.0666e-6f;

float io_power_monitor_read_power(const PowerRail *power_address)
{
    uint8_t buffer[2];

    io_power_monitor_refresh();

    buffer[0] = power_address->power_address;
    hw_i2c_transmit(&pwr_mon_i2c, buffer, 1); // change register pointer address

    hw_i2c_receive(&pwr_mon_i2c, buffer, 2); // get raw register data and store it in buffer

    const float voltage_power_buffer = (float)((buffer[0] << 8) | buffer[1]) * VOLTAGE_POWER_TRANSFER_FACTOR;

    return voltage_power_buffer * POWER_FSR;
}

#define ALERT_STATUS 0x26

uint32_t io_power_monitor_alert_status()
{
    uint8_t buffer[3]; // Does not require a REFRESH

    buffer[0] = ALERT_STATUS;                 // ALERT_STATUS address
    hw_i2c_transmit(&pwr_mon_i2c, buffer, 1); // change register pointer to alert_status

    hw_i2c_receive(&pwr_mon_i2c, buffer, 3); // Gets all the alert statuses

    const uint32_t alert_buffer =
        (uint32_t)((buffer[0] << 16) | (buffer[1] << 8) | buffer[2]); // 23:0 bits for alert (datasheet)

    return alert_buffer;
}
