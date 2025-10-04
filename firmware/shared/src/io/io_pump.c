#include <assert.h>

#include "io_pump.h"

#include "app_utils.h"
#include "io_loadswitch.h"
#include "io_potentiometer.h"

static const PumpConfig* pump_table = NULL;
static size_t            pump_count = 0u;

void io_pumps_registerConfig(const PumpConfig *table, size_t count)
{
    assert(table != NULL || count == 0u);
    pump_table = table;
    pump_count = count;
}

static const PumpConfig* lookupPump(PumpID id)
{
    if (pump_table == NULL || id >= pump_count) {
        assert(false);
        return NULL;
    }
    return &pump_table[id];
}

static uint8_t logicalToHw(uint8_t logical_percent, const PumpConfig *cfg)
{
return cfg->invert ? (uint8_t)(100u - logical_percent) : logical_percent;
}

static uint8_t hwToLogical(uint8_t hw_percent, const PumpConfig *cfg)
{
return cfg->invert ? (uint8_t)(100u - hw_percent) : hw_percent;
}

ExitCode io_rPump_setPercentage(uint8_t percentage, PumpID id)
{
    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->pot == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const uint8_t hw_percent = logicalToHw(percentage, id);
    return io_potentiometer_writePercentage(cfg->pot, cfg->wiper, hw_percent);
}

ExitCode io_rPump_getPercentage(uint8_t percentage, PumpID id)
{
    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->pot == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const uint8_t hw_percent = logicalToHw(percentage, id);
    return io_potentiometer_writePercentage(cfg->pot, cfg->wiper, hw_percent);
}

ExitCode io_pump_enable(PumpID id, bool enable)
{
    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->efuse == NULL)
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    io_loadswitch_setChannel(cfg->efuse, enable);
    return EXIT_CODE_OK;
}

ExitCode io_pump_isEnabled(PumpID id, bool *enabled_out)
{
    if (enabled_out == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->efuse == NULL)
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    *enabled_out = io_loadswitch_isChannelEnabled(cfg->efuse);
    return EXIT_CODE_OK;
}




