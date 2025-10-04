#include "io_efuse.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

void io_efuse_setChannel(const Efuse *channel, const bool enabled)
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

void io_efuse_reset_set(const Efuse *channel, const bool set)
{
    assert(channel != NULL);
    channel->efuse_functions->loadswitch_reset_set(channel);
}

void io_efuse_reset(const Efuse *channel)
{
    assert(channel != NULL);
    channel->efuse_functions->reset_efuse(channel);
}
