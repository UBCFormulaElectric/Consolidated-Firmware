#include "io_loadswitchFake.h"

#include <cassert>

namespace fake::io_loadswitch
{
void simulate_st_loadswitch_fault(const ST_LoadSwitch *loadswitch, const Efuse *efuse)
{
    // first, note that the switch must be on for a fault to be simulated
    assert(loadswitch->efuse1 == efuse || loadswitch->efuse2 == efuse);
    assert(efuse->enabled);

    // divide by a random number between
    const_cast<Efuse *>(efuse)->simulate_fault = true;
}
void simulate_ti_loadswitch_fault(const TI_LoadSwitch *loadswitch)
{
    // first, note that the switch must be on for a fault to be simulated
    assert(loadswitch->efuse->enabled);

    const_cast<Efuse *>(const_cast<TI_LoadSwitch *>(loadswitch)->efuse)->simulate_fault = true;
    const_cast<TI_LoadSwitch *>(loadswitch)->pgood                                      = false;
}
} // namespace fake::io_loadswitch

extern "C"
{
    void io_loadswitch_setChannel(const Efuse *channel, const bool enabled)
    {
        const_cast<Efuse *>(channel)->enabled = enabled;

        // todo simulate turning on efuse
        // in particular simulate how successful it is
    }
    bool io_loadswitch_isChannelEnabled(const Efuse *channel)
    {
        return channel->enabled;
    }
    float io_loadswitch_getChannelCurrent(const Efuse *channel)
    {
        // todo simulate current tick
        return channel->current;
    }

    void io_STloadswitch_Reset(const ST_LoadSwitch *loadswitch)
    {
        io_loadswitch_setChannel(loadswitch->efuse1, true);
        io_loadswitch_setChannel(loadswitch->efuse2, true);
    }
    void io_TILoadswitch_Reset(const TI_LoadSwitch *loadSwitch)
    {
        io_loadswitch_setChannel(loadSwitch->efuse, true);
    }

    bool io_TILoadswitch_pgood(const TI_LoadSwitch *loadSwitch)
    {
        return loadSwitch->pgood;
    }
}