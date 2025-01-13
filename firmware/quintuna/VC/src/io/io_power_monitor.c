#include "io_power_monitor.h"
#include "hw_i2cs.h"

enum voltage_bus {
 	VBAT = 0x0B,
    V_BOOST = 0x0C,
    V_ACC = 0x0D,
    V_EXT = 0x0E
};

bool io_power_monitor_init() {
    return hw_i2c_isTargetReady(&pwr_mon);
}

int io_power_monitor_read(voltage_bus voltage) { //might change to uint8_t
    //refresh command not needed (send byte command does thee same and changes pointer to correct register address)
    //change the hw_i2c to accept both send byte and recieve byte (NVM MIGHT NOT BE NEEDED JUST NOTICED)
    return 0;
}

// am I missing anything else for the poer monitoring chip??
