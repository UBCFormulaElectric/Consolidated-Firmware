#include "io_pump.h"
#include "io_potentiometer.h"
#include <stdint.h>
#include "hw_i2cs.h"

static const Potentiometer pump_pot = { .i2c_handle = &pumps };

ExitCode io_pumpControl_setPercentage(uint8_t percentage, PUMPS pump)
{
    assert(pump == RR_PUMP || pump == F_PUMP);

    POTENTIOMETER_WIPER wiper = (POTENTIOMETER_WIPER)((pump == F_PUMP) ? WIPER0 : WIPER1);
    return io_potentiometer_writePercentage(&pump_pot, wiper, percentage);
}

ExitCode io_pumpControl_readPercentage(uint8_t *percentage, PUMPS pump)
{
    assert(pump == RR_PUMP || pump == F_PUMP);

    POTENTIOMETER_WIPER wiper = (POTENTIOMETER_WIPER)((pump == F_PUMP) ? WIPER0 : WIPER1);
    return io_potentiometer_readPercentage(&pump_pot, wiper, percentage);
}