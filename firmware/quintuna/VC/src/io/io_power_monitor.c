#include "io_power_monitor.h"
#include "hw_i2cs.h"

//make a enum matrix smth to have the 4 diff types power mon Volt readings

enum voltage_bus {
 	VBAT = 0x0B,
    V_BOOST = 0x0C,
    V_ACC = 0x0D,
    V_EXT = 0x0E
};

bool io_power_monitor_init() {
    return hw_i2c_isTargetReady(&pwr_mon);
}

uint8_t io_power_monitor_read(voltage_bus voltage) {
    //refresh command not needed (send byte command does thee same and changes pointer to correct register address)


}
