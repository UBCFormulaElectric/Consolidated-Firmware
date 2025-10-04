#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "app_utils.h"
#include "io_loadswitch.h"
#include "io_potentiometer.h"

typedef enum
{
    RR_PUMP = 0,
    RL_PUMP,
    F_PUMP,
    PUMP_COUNT
} PumpID;

typedef struct
{
    const Potentiometer *pot;
    POTENTIOMETER_WIPER  wiper;
    bool                  invert;
    const Efuse          *efuse;
    const TI_LoadSwitch  *ti_loadswitch;
} PumpConfig;

void io_pumps_registerConfig(const PumpConfig *table, size_t count);

ExitCode io_pump_setPercent(PumpID id, uint8_t percent);
ExitCode io_pump_getPercent(PumpID id, uint8_t *percent_out);
ExitCode io_pump_enable(PumpID id, bool enable);
ExitCode io_pump_isEnabled(PumpID id, bool *enabled_out);
ExitCode io_pump_getHealth(PumpID id, bool *pgood_out);
