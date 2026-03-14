#include "app_loadswitches.hpp"
#include "app_canTx.hpp"
#include "io_efuse.hpp"

/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the
current we want to send over CAN for each efuse channel.
*/

namespace app::loadswitches
{
struct EfuseCanMsg
{
    io::Efuse *efuse;
    void (*can_setter_enabled)(bool);
    void (*can_setter_current)(float);
};
// one array for all the setter CAN signals
static const std::array<EfuseCanMsg, NUM_EFUSE_CHANNELS> efuse_channel_setters = { {
    [F_INV_Efuse]   = { &io::loadswitches::F_INV_Efuse, app::can_tx::VC_FrontInvertersStatus_set,
                        app::can_tx::VC_FrontInvertersCurrent_set },
    [RSM_Efuse]     = { &io::loadswitches::RSM_Efuse, app::can_tx::VC_RSMStatus_set, app::can_tx::VC_RSMCurrent_set },
    [BMS_Efuse]     = { &io::loadswitches::BMS_Efuse, app::can_tx::VC_BMSStatus_set, app::can_tx::VC_BMSCurrent_set },
    [R_INV_Efuse]   = { &io::loadswitches::R_INV_Efuse, app::can_tx::VC_RearInvertersStatus_set,
                        app::can_tx::VC_RearInvertersCurrent_set },
    [DAM_Efuse]     = { &io::loadswitches::DAM_Efuse, app::can_tx::VC_DAMStatus_set, app::can_tx::VC_DAMCurrent_set },
    [FRONT_Efuse]   = { &io::loadswitches::FRONT_Efuse, app::can_tx::VC_FrontStatus_set,
                        app::can_tx::VC_FrontCurrent_set },
    [RL_PUMP_Efuse] = { &io::loadswitches::RL_PUMP_Efuse, app::can_tx::VC_RearLeftPumpStatus_set,
                        app::can_tx::VC_RearLeftPumpCurrent_set },
    [R_RAD_Efuse]   = { &io::loadswitches::R_RAD_Efuse, app::can_tx::VC_RightRadiatorFanStatus_set,
                        app::can_tx::VC_RightRadiatorFanCurrent_set },
    [RR_PUMP_Efuse] = { &io::loadswitches::RR_PUMP_Efuse, app::can_tx::VC_RearRightPumpStatus_set,
                        app::can_tx::VC_RearRightPumpCurrent_set },
    [L_RAD_Efuse]   = { &io::loadswitches::L_RAD_Efuse, app::can_tx::VC_LeftRadiatorFanStatus_set,
                        app::can_tx::VC_LeftRadiatorFanCurrent_set },
} };

void efuse_broadcast()
{
    // run through each efuse, and broadcast the channel status and current
    for (const auto &efuse : efuse_channel_setters)
    {
        assert(efuse.efuse != nullptr);

        const bool  enabled = efuse.efuse->isChannelEnabled();
        const float current = efuse.efuse->getChannelCurrent();
    }
}
} // namespace app::loadswitches