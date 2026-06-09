#pragma once
#include <array>
#include <cstdint>
#include <cassert>
#include "app_timer.hpp"
#include "app_canAlerts.hpp"
#include "io_efuse.hpp"
#include "io_efuses.hpp"

namespace app::powerManager
{

struct EfuseConfig
{
    bool     efuse_enable{ false };
    uint32_t timeout{ 0 };
    uint8_t  max_retry{ 0 };
};

struct PowerManagerConfig
{
    EfuseConfig front_efuse;
    EfuseConfig rsm_efuse;
    EfuseConfig bms_efuse;
    EfuseConfig dam_efuse;
    EfuseConfig f_inv_efuse;
    EfuseConfig r_inv_efuse;
    EfuseConfig r_rad_fan_efuse;
    EfuseConfig l_rad_fan_efuse;
    EfuseConfig rr_pump_efuse;
    EfuseConfig rl_pump_efuse;

    std::span<const EfuseConfig> as_span() const
    {
        return std::span{ &rr_pump_efuse, sizeof(this) / sizeof(EfuseConfig) };
    }
};

void updateConfig(const PowerManagerConfig &new_cfg);
void efuseProtocolTick_100Hz();
void broadcastRetryCounts();

#ifdef TARGET_TEST
PowerManagerConfig getConfig();
bool               getEfuse(const int Ch);
#endif
} // namespace app::powerManager
