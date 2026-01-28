#pragma once
namespace vc::app::startSwitch
{
bool app_startSwitch_hasRisingEdge(void);
typedef enum
{
    SWITCH_OFF               = 0,
    SWITCH_ON                = 1,
    NUM_SWITCH_STATE_CHOICES = 2,
} SwitchState;
} // namespace vc::app::startSwitch