#include <assert.h>

#include "io_pump.h"

static const PumpConfig* pump_table[PUMP_COUNT] = { NULL };
static size_t pump_count = 0u;

static ExitCode requirePumpResource(const PumpConfig *cfg, const void *resource, ExitCode err_code)
{
    if (cfg == NULL || resource == NULL)
    {
        return err_code;
    }
    return EXIT_CODE_OK;
}

void io_pumps_init(const PumpConfig *table, size_t count)
{
    assert(table != NULL || count == 0u);
    assert(count <= PUMP_COUNT);

    for (size_t i = 0u; i < count; i++)
    {
        pump_table[i] = &table[i];
    }
    for (size_t i = count; i < PUMP_COUNT; i++)
    {
        pump_table[i] = NULL;
    }

    pump_count = count;
}

void io_pumps_registerConfig(PumpConfig *const table[], size_t count)
{
    assert(table != NULL || count == 0u);
    assert(count <= PUMP_COUNT);

    for (size_t i = 0u; i < count; ++i) {
        pump_table[i] = table[i];
    }
    for (size_t i = count; i < PUMP_COUNT; ++i) {
        pump_table[i] = NULL;
    }

    pump_count = count;
}

static const PumpConfig* lookupPump(PumpID id)
{
    if (id >= pump_count || pump_table[id] == NULL) {
        assert(false);
        return NULL;
    }
    return pump_table[id];
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
    ExitCode status = requirePumpResource(cfg, cfg != NULL ? cfg->pot : NULL, EXIT_CODE_INVALID_ARGS);
    if (status != EXIT_CODE_OK)
    {
        return status;
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
    ExitCode status = requirePumpResource(cfg, cfg != NULL ? cfg->pot : NULL, EXIT_CODE_INVALID_ARGS);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    uint8_t hw_percent = 0u;
    status = io_potentiometer_readPercentage(cfg->pot, cfg->wiper, &hw_percent);
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
    ExitCode status = requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    io_efuse_setChannel(cfg->efuse, enable);
    return EXIT_CODE_OK;
}

ExitCode io_pump_isEnabled(PumpID id, bool *enabled_out)
{
    if (enabled_out == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const PumpConfig *cfg = lookupPump(id);
    ExitCode          status =
        requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    *enabled_out = io_efuse_isChannelEnabled(cfg->efuse);
    return EXIT_CODE_OK;
}

ExitCode io_pump_getHealth(PumpID id, bool *pgood_out)
{
    if (pgood_out == NULL)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    const PumpConfig *cfg = lookupPump(id);
    ExitCode          status =
        requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    *pgood_out = io_efuse_pgood(cfg->efuse);
    return EXIT_CODE_OK;
}
