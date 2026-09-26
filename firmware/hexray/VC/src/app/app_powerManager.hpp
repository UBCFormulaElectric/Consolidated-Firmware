#pragma once
#include "app_canAlerts.hpp"

#include <cstdint>
#include <cassert>
#include "main.h"

namespace app::powerManager
{

struct EfuseConfig
{
    bool     efuse_enable{ false };
    uint32_t timeout{ 0 };
    uint8_t  max_retry{ 0 };
};

// Order matches the efuses struct below
enum class Efuse_E : uint8_t
{
    FRONT,
    RSM,
    BMS,
    DAM,
    F_INV,
    R_INV,
    R_RAD_FAN,
    L_RAD_FAN,
    RR_PUMP,
    RL_PUMP,
};

template <typename T> struct Efuses
{
    T front_efuse;
    T rsm_efuse;
    T bms_efuse;
    T dam_efuse;
    T f_inv_efuse;
    T r_inv_efuse;
    T r_rad_fan_efuse;
    T l_rad_fan_efuse;
    T rr_pump_efuse;
    T rl_pump_efuse;

    std::span<const T> as_span() const
    {
        return std::span{ reinterpret_cast<const T *>(this), sizeof(*this) / sizeof(T) };
    }

    T operator[](size_t idx) const
    {
        assert(idx < 10);
        return as_span()[idx];
    }
};

void updateConfig(const Efuses<EfuseConfig> &new_cfg);
void efuseProtocolTick_100Hz();
void broadcastRetryCounts();

#ifdef TARGET_TEST
Efuses<EfuseConfig> getConfig();
bool                getEfuse(const int Ch);
#endif
} // namespace app::powerManager
