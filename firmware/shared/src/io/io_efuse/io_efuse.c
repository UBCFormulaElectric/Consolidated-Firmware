#include "io_efuse.h"
// #include "io_efuse_datatypes.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

void io_efuse_setChannel(const Efuse *channel, bool enabled)
{
    assert(channel != NULL);

    channel->efuse_functions->set_channel(channel, enabled);
}

bool io_efuse_isChannelEnabled(const Efuse *channel)
{
    assert(channel != NULL);
    return channel->efuse_functions->is_channel_enabled(channel);
}

float io_efuse_getChannelCurrent(const Efuse *channel)
{
    assert(channel != NULL);
    return channel->efuse_functions->get_channel_current(channel);
}

void io_efuse_reset_set(const Efuse *channel, bool set)
{
    assert(channel != NULL);
    assert(channel->efuse_functions->loadswitch_reset_set != NULL);

    channel->efuse_functions->loadswitch_reset_set(channel, set);
}

void io_efuse_reset(const Efuse *channel)
{
    assert(channel != NULL);
    channel->efuse_functions->reset_efuse(channel);
}

bool io_efuse_pgood(const Efuse *channel)
{
    assert(channel != NULL);
    assert(channel->efuse_functions->pgood != NULL);

    return channel->efuse_functions->pgood(channel);
}

bool io_efuse_ok(const Efuse *efuse)
{
    assert(efuse != NULL);
    return IS_EXIT_OK(efuse->efuse_functions->efuse_ok(efuse));
}

void io_efuse_setDiagnostics(const Efuse *efuse, bool enabled)
{
    assert(efuse != NULL);
    assert(efuse->efuse_functions->set_diagnostics != NULL);

    efuse->efuse_functions->set_diagnostics(efuse, enabled);
}
