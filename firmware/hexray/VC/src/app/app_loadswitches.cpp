#include "app_loadswitches.hpp"
#include "app_canTx.hpp"
#include "io_efuse.hpp"
#include "io_loadswitches.hpp"
#include <array>

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
// static const std::array<EfuseCanMsg, NUM_EFUSE_CHANNELS> efuse_channel_setters = { {
//     [F_INV]   = { &F_INV_Efuse, app::can_tx::VC_FrontInvertersStatus_set,
//                   app::can_tx::VC_FrontInvertersCurrent_set },
//     [RSM]     = { &RSM_Efuse, app::can_tx::VC_RSMStatus_set, app::can_tx::VC_RSMCurrent_set },
//     [BMS]     = { &BMS_Efuse, app::can_tx::VC_BMSStatus_set, app::can_tx::VC_BMSCurrent_set },
//     [R_INV]   = { &R_INV_Efuse, app::can_tx::VC_RearInvertersStatus_set,
//                   app::can_tx::VC_RearInvertersCurrent_set },
//     [DAM]     = { &DAM_Efuse, app::can_tx::VC_DAMStatus_set, app::can_tx::VC_DAMCurrent_set },
//     [FRONT]   = { &FRONT_Efuse, app::can_tx::VC_FrontStatus_set, app::can_tx::VC_FrontCurrent_set },
//     [RL_PUMP] = { &RL_PUMP_Efuse, app::can_tx::VC_RearLeftPumpStatus_set,
//                   app::can_tx::VC_RearLeftPumpCurrent_set },
//     [R_RAD]   = { &R_RAD_Efuse, app::can_tx::VC_RightRadiatorFanStatus_set,
//                   app::can_tx::VC_RightRadiatorFanCurrent_set },
//     [RR_PUMP] = { &RR_PUMP_Efuse, app::can_tx::VC_RearRightPumpStatus_set,
//                   app::can_tx::VC_RearRightPumpCurrent_set },
//     [L_RAD]   = { &L_RAD_Efuse, app::can_tx::VC_LeftRadiatorFanStatus_set,
//                   app::can_tx::VC_LeftRadiatorFanCurrent_set },
// } };

constexpr std::size_t idx(EfuseChannel ch)
{
    return static_cast<std::size_t>(ch);
}
static const std::array<EfuseCanMsg, NUM_EFUSE_CHANNELS> efuse_channel_setters = []
{
    std::array<EfuseCanMsg, NUM_EFUSE_CHANNELS> arr{};

    arr[idx(EfuseChannel::F_INV)] = { &F_INV_Efuse, app::can_tx::VC_FrontInvertersStatus_set,
                                      app::can_tx::VC_FrontInvertersCurrent_set };

    arr[idx(EfuseChannel::RSM)] = { &RSM_Efuse, app::can_tx::VC_RSMStatus_set, app::can_tx::VC_RSMCurrent_set };

    arr[idx(EfuseChannel::BMS)] = { &BMS_Efuse, app::can_tx::VC_BMSStatus_set, app::can_tx::VC_BMSCurrent_set };

    arr[idx(EfuseChannel::R_INV)] = { &R_INV_Efuse, app::can_tx::VC_RearInvertersStatus_set,
                                      app::can_tx::VC_RearInvertersCurrent_set };

    arr[idx(EfuseChannel::DAM)] = { &DAM_Efuse, app::can_tx::VC_DAMStatus_set, app::can_tx::VC_DAMCurrent_set };

    arr[idx(EfuseChannel::FRONT)] = { &FRONT_Efuse, app::can_tx::VC_FrontStatus_set, app::can_tx::VC_FrontCurrent_set };

    arr[idx(EfuseChannel::RL_PUMP)] = { &RL_PUMP_Efuse, app::can_tx::VC_RearLeftPumpStatus_set,
                                        app::can_tx::VC_RearLeftPumpCurrent_set };

    arr[idx(EfuseChannel::R_RAD)] = { &R_RAD_Efuse, app::can_tx::VC_RightRadiatorFanStatus_set,
                                      app::can_tx::VC_RightRadiatorFanCurrent_set };

    arr[idx(EfuseChannel::RR_PUMP)] = { &RR_PUMP_Efuse, app::can_tx::VC_RearRightPumpStatus_set,
                                        app::can_tx::VC_RearRightPumpCurrent_set };

    arr[idx(EfuseChannel::L_RAD)] = { &L_RAD_Efuse, app::can_tx::VC_LeftRadiatorFanStatus_set,
                                      app::can_tx::VC_LeftRadiatorFanCurrent_set };

    return arr;
}();

void efuse_broadcast()
{
    // run through each efuse, and broadcast the channel status and current
    for (const auto &efuse : efuse_channel_setters)
    {
        assert(efuse.efuse != nullptr);

        const bool enabled = efuse.efuse->isChannelEnabled();
        efuse.can_setter_enabled(enabled);
        const float current = efuse.efuse->getChannelCurrent();
        efuse.can_setter_current(current);
    }
}
} // namespace app::loadswitches