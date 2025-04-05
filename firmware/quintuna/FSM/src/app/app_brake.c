#include "app_brake.h"
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_brake.h"
#include <math.h>

static const RangeCheck front_pressure_in_range_check = { .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };
static const RangeCheck rear_pressure_in_range_check  = { .min_value = MIN_BRAKE_PRESSURE_PSI,
                                                          .max_value = MAX_BRAKE_PRESSURE_PSI };

void app_brake_broadcast(void)
{
    const bool brake_pressed = io_brake_isActuated();
    // TODO: send can message of brake actuation

    float front_pressure = io_brake_getFrontPressurePsi();
    RangeCheckStatusMetaData front_pressure_status = app_rangeCheck_getValue(&front_pressure_status, front_pressure);
    // TODO: send can message of brake pressure ((uint32_t)roundf(front_pressure))
    // TODO: send can alert (warning) of front brake pressure being out of range -> front_pressure_status.status != VALUE_IN_RANGE

    // TODO: send can alert (warning) of front brake pressure ocsc being triggered -> io_brake_frontPressureSensorOCSC()
    // TODO: send can alert (warning) of hardware brake ocsc being triggered -> io_brake_hwOCSC()
}
