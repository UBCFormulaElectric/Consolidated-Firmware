#include "app_powerManager.hpp"
#include "io_tps28_efuse.hpp"
#include "io_tps25_efuse.hpp"
#include "io_efuse.hpp"

// Defined in the loadswitches io layer
static app::PowerManager powerManager({
    &RR_PUMP_Efuse,
    &RL_PUMP_Efuse,
    &R_RAD_Efuse,
    &L_RAD_Efuse,
    &F_INV_Efuse,
    &R_INV_Efuse,
    &RSM_Efuse,
    &BMS_Efuse,
    &DAM_Efuse,
    &FRONT_Efuse,
});

void init()
{
    powerManager.init();
}

void updateConfig(app::PowerManagerConfig cfg)
{
    powerManager.updateConfig(cfg);
}

void efuseProtocolTick_100Hz()
{
    powerManager.efuseProtocolTick_100Hz();
}