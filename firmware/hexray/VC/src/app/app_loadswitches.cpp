#include "app_loadswitches.hpp"
#include "app_canTx.hpp"
#include "io_efuse.hpp"

/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the
current we want to send over CAN for each efuse channel.
*/

namespace app::loadswitches
{
    // array passed in will be the efuse_channels which will have all the efuses.
    static void (*const efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
        [F_INV_Efuse]   = app::can_tx::VC_FrontInvertersStatus_set,
        [RSM_Efuse]     = app::can_tx::VC_RSMStatus_set,
        [BMS_Efuse]     = app::can_tx::VC_BMSStatus_set,
        [R_INV_Efuse]   = app::can_tx::VC_RearInvertersStatus_set,
        [DAM_Efuse]     = app::can_tx::VC_DAMStatus_set,
        [FRONT_Efuse]   = app::can_tx::VC_FrontStatus_set,
        [RL_PUMP_Efuse] = app::can_tx::VC_RearLeftPumpStatus_set,
        [R_RAD_Efuse]   = app::can_tx::VC_RightRadiatorFanStatus_set,
        [RR_PUMP_Efuse] = app::can_tx::VC_RearRightPumpStatus_set,
        [L_RAD_Efuse]   = app::can_tx::VC_LeftRadiatorFanStatus_set,
    };

    static void (*const efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
        [F_INV_Efuse]   = app::can_tx::VC_FrontInvertersCurrent_set,
        [RSM_Efuse]     = app::can_tx::VC_RSMCurrent_set,
        [BMS_Efuse]     = app::can_tx::VC_BMSCurrent_set,
        [R_INV_Efuse]   = app::can_tx::VC_RearInvertersCurrent_set,
        [DAM_Efuse]     = app::can_tx::VC_DAMCurrent_set,
        [FRONT_Efuse]   = app::can_tx::VC_FrontCurrent_set,
        [RL_PUMP_Efuse] = app::can_tx::VC_RearLeftPumpCurrent_set,
        [R_RAD_Efuse]   = app::can_tx::VC_RightRadiatorFanCurrent_set,
        [RR_PUMP_Efuse] = app::can_tx::VC_RearRightPumpCurrent_set,
        [L_RAD_Efuse]   = app::can_tx::VC_LeftRadiatorFanCurrent_set,
    };

    void efuse_broadcast(void)
    {
        // run through each efuse, and broadcast the channel status and current
        for (int i = 0; i < NUM_EFUSE_CHANNELS; i++)
        {
            assert(efuse_channels[i] != nullptr);

            const bool  enabled = efuse_channels[i]->isChannelEnabled();
            const float current = efuse_channels[i]->getChannelCurrent();
            efuse_enabled_can_setters[i](enabled);
            efuse_current_can_setters[i](current);
        }
    }
} // namespace app::loadswitches