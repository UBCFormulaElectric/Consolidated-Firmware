#pragma once
namespace app::startSwitch
{
bool app_startSwitch_hasRisingEdge(void);
enum class SwitchState : uint8_t
{
    SWITCH_OFF = 0,
    SWITCH_ON  = 1,
};
} // namespace app::startSwitch