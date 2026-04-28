#include "app_loadswitches.hpp"
#include "app_canTx.hpp"
#include "io_efuses.hpp"
#include "io_efuse.hpp"
#include <array>

/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the
current we want to send over CAN for each efuse channel.
*/
namespace app::loadswitches
{
struct EfuseCanMsg
{
    io::Efuse &efuse;
    void (*can_setter_enabled)(bool);
    void (*can_setter_current)(float);
};

// one array for all the setter CAN signals
EfuseCanMsg efuse_channel_setters[NUM_EFUSE_CHANNELS] = {
    { f_inv_efuse, app::can_tx::VC_FrontInvertersStatus_set, app::can_tx::VC_FrontInvertersCurrent_set },
    { r_inv_efuse, app::can_tx::VC_RSMStatus_set, app::can_tx::VC_RSMCurrent_set },
    { bms_efuse, app::can_tx::VC_BMSStatus_set, app::can_tx::VC_BMSCurrent_set },
    { rsm_efuse, app::can_tx::VC_RearInvertersStatus_set, app::can_tx::VC_RearInvertersCurrent_set },
    { dam_efuse, app::can_tx::VC_DAMStatus_set, app::can_tx::VC_DAMCurrent_set },
    { front_efuse, app::can_tx::VC_FrontStatus_set, app::can_tx::VC_FrontCurrent_set },
    { l_rad_fan_efuse, app::can_tx::VC_RearLeftPumpStatus_set, app::can_tx::VC_RearLeftPumpCurrent_set },
    { r_rad_fan_efuse, app::can_tx::VC_RightRadiatorFanStatus_set, app::can_tx::VC_RightRadiatorFanCurrent_set },
    { rr_pump_efuse, app::can_tx::VC_RearRightPumpStatus_set, app::can_tx::VC_RearRightPumpCurrent_set },
    { rl_pump_efuse, app::can_tx::VC_LeftRadiatorFanStatus_set, app::can_tx::VC_LeftRadiatorFanCurrent_set },
};

void efuse_broadcast()
{
    // run through each efuse, and broadcast the channel status and current
    for (auto &efuse : efuse_channel_setters)
    {
        efuse.can_setter_enabled(efuse.efuse.isChannelEnabled());
        efuse.can_setter_current(efuse.efuse.getChannelCurrent());
    }
}
} // namespace app::loadswitches