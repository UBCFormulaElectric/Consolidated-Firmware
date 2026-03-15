#include "app_powerManager.hpp"
#include "io_tps28_efuse.hpp"
#include "io_tps25_efuse.hpp"
#include "io_efuse.hpp"

// Defined in the loadswitches io layer
static app::PowerManager powerManager({
    &io::loadswitches::RR_PUMP_Efuse,
    &io::loadswitches::RL_PUMP_Efuse,
    &io::loadswitches::R_RAD_Efuse,
    &io::loadswitches::L_RAD_Efuse,
    &io::loadswitches::F_INV_Efuse,
    &io::loadswitches::R_INV_Efuse,
    &io::loadswitches::RSM_Efuse,
    &io::loadswitches::BMS_Efuse,
    &io::loadswitches::DAM_Efuse,
    &io::loadswitches::FRONT_Efuse,
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