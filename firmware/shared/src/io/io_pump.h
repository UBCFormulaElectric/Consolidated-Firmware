#pragma once 

#include <stdint.h>
#include <stdbool.h>

#include "io_potentiometer.h"
#include "io_loadswitch.h"

typedef struct
{
    const Potentiometer *pot;
    POTENTIOMETER_WIPER wiper;
    bool invert;
    const Efuse *efuse;
    const TI_LoadSwitch *ti_loadswitch;

} PumpConfig;

typedef enum 
{
    RR_PUMP = 0,
    RL_PUMP, 
    F_PUMP,
    PUMP_COUNT
} PumpID;

void io_pumps_registerConfig(const PumpConfig *table, size_t count);
static uint8_t logicalToHw(uint8_t logical_percent, const PumpConfig *cfg);
static uint8_t hwToLogical(uint8_t hw_percent, const PumpConfig *cfg);
ExitCode io_rPump_setPercentage(uint8_t percentage, PumpID id);
ExitCode io_rPump_getPercentage(uint8_t percentage, PumpID id);
ExitCode io_pump_enable(PumpID id, bool enable);
ExitCode io_pump_isEnabled(PumpID id, bool *enabled_out);



