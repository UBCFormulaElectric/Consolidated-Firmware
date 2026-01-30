#include "app_loadswitches.hpp"
/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the
current we want to send over CAN for each efuse channel.
*/
static const void efuse_channel_setter(EfuseChannel &channel, bool status)
{
    switch (channel)
    {
        case EfuseChannel::TI_EfuseChannel::F_INV:
            app_canTx_VC_FrontInvertersStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::R_INV:
            app_canTx_VC_RearInvertersStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::RSM:
            app_canTx_VC_RSMStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::BMS:
            app_canTx_VC_BMSStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::DAM:
            app_canTx_VC_DAMStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::FRONT:
            app_canTx_VC_FrontStatus_set(status);
            break;
        case EfuseChannel::TI_EfuseChannel::MISC:
            app_canTx_VC_MiscStatus_set(status);
            break;
        case EfuseChannel::ST_EfuseChannel::RR_PUMP:
            app_canTx_VC_RearRightPumpStatus_set(status);
            break;
        case EfuseChannel::ST_EfuseChannel::RL_PUMP:
            app_canTx_VC_RearLeftPumpStatus_set(status);
            break;
        case EfuseChannel::ST_EfuseChannel::R_RAD:
            app_canTx_VC_RightRadiatorFanStatus_set(status);
            break;
        case EfuseChannel::ST_EfuseChannel::L_RAD:
            app_canTx_VC_LeftRadiatorFanStatus_set(status);
            break;
    }
}

static const void efuse_current_setter(EfuseChannel &channel, float *current)
{
    switch (channel)
    {
        case EfuseChannel::TI_EfuseChannel::F_INV:
            app_canTx_VC_FrontInvertersCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::R_INV:
            app_canTx_VC_RearInvertersCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::RSM:
            app_canTx_VC_RSMCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::BMS:
            app_canTx_VC_BMSCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::DAM:
            app_canTx_VC_DAMCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::FRONT:
            app_canTx_VC_FrontCurrent_set(current);
            break;
        case EfuseChannel::TI_EfuseChannel::MISC:
            app_canTx_VC_MiscCurrent_set(current);
            break;
        case EfuseChannel::ST_EfuseChannel::RR_PUMP:
            app_canTx_VC_RearRightPumpCurrent_set(current);
            break;
        case EfuseChannel::ST_EfuseChannel::RL_PUMP:
            app_canTx_VC_RearLeftPumpCurrent_set(current);
            break;
        case EfuseChannel::ST_EfuseChannel::R_RAD:
            app_canTx_VC_RightRadiatorFanCurrent_set(current);
            break;
        case EfuseChannel::ST_EfuseChannel::L_RAD:
            app_canTx_VC_LeftRadiatorFanCurrent_set(current);
            break;
    }
}
