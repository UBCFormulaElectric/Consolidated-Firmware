#include "app_batteryCharging.hpp"
#include "io_batteryCharging.hpp"
#include "io_batteryMonitoring.hpp"

namespace app::batteryCharging
{
std::expected<void, ErrorCode> update()
{
    static bool charging_enabled = false;

    const auto ov_uv_fault_active = io::batteryMonitoring::is_cell_ov_uv_fault_active();
    RETURN_IF_ERR(ov_uv_fault_active);

    if (*ov_uv_fault_active)
    {
        if (charging_enabled)
        {
            io::batteryCharging::chargerDisable();
            charging_enabled = false;
            LOG_INFO("Charging disabled due to cell OV/UV fault");
        }
    }
    else
    {
        if (!charging_enabled)
        {
            io::batteryCharging::chargerEnable();
            charging_enabled = true;
            LOG_INFO("Charging enabled");
        }
    }

    return {};
}
} // namespace app::batteryCharging