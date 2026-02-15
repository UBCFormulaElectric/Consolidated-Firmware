#pragma once
namespace app::startSwitch
{
enum class SwitchState : uint8_t
{
    SWITCH_OFF = 0,
    SWITCH_ON  = 1,
};

bool hasRisingEdge(void);
} // namespace app::startSwitch