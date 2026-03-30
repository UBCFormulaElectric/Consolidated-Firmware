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
    bool    efuse_enable{ false };
    uint8_t timeout{ 0 };
    uint8_t max_retry{ 0 };
};

struct PowerManagerConfig
{
    std::array<EfuseConfig, NUM_EFUSE_CHANNELS> efuse_configs{};
};

void init(const std::array<io::Efuse *, NUM_EFUSE_CHANNELS> &efuses);
void updateConfig(const PowerManagerConfig &new_cfg);
void efuseProtocolTick_100Hz();

#ifdef TARGET_TEST
PowerManagerConfig getConfig();
bool               getEfuse(const int Ch);
#endif
} // namespace app::powerManager
