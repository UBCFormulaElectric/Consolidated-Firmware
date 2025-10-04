#include <assert.h>

#include "io_pump.h"

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

static uint8_t logicalToHw(const PumpConfig *cfg, uint8_t logical_percent)
{
    return cfg->invert ? (uint8_t)(100u - logical_percent) : logical_percent;
}

static uint8_t hwToLogical(const PumpConfig *cfg, uint8_t hw_percent)
{
    return cfg->invert ? (uint8_t)(100u - hw_percent) : hw_percent;
}

ExitCode io_pump_setPercent(PumpID id, uint8_t percent)
{
    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->pot == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const uint8_t hw_percent = logicalToHw(cfg, percent);
    return io_potentiometer_writePercentage(cfg->pot, cfg->wiper, hw_percent);
}

ExitCode io_pump_getPercent(PumpID id, uint8_t *percent_out)
{
    if (percent_out == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->pot == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    uint8_t hw_percent = 0u;
    ExitCode status    = io_potentiometer_readPercentage(cfg->pot, cfg->wiper, &hw_percent);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    *percent_out = hwToLogical(cfg, hw_percent);
    return EXIT_CODE_OK;
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

ExitCode io_pump_getHealth(PumpID id, bool *pgood_out)
{
    if (pgood_out == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const PumpConfig *cfg = lookupPump(id);
    if (cfg == NULL || cfg->ti_loadswitch == NULL)
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    *pgood_out = io_TILoadswitch_pgood(cfg->ti_loadswitch);
    return EXIT_CODE_OK;
}



