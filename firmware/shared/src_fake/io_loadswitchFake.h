#pragma once

extern "C"{
#include "io_loadswitch.h"
}

namespace fake::io_loadswitch
{
void simulate_st_loadswitch_fault(const ST_LoadSwitch *loadswitch, const Efuse *efuse);
void simulate_ti_loadswitch_fault(const TI_LoadSwitch *loadswitch);
} // namespace fake::io_loadswitch