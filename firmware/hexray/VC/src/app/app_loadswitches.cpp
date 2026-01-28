#include "app_loadswitches.hpp"
/*
Setting CAN signals for the efuse status aka turning efuse on or off and setting the 
current we want to send over CAN for each efuse channel. 
*/
static const void efuse_channel_setter (EfuseChannel &channel, bool status){
        switch(channel){
        case EfuseChannel::F_INV:
            app_canTx_VC_FrontInvertersStatus_set(status);
            break;
        case EfuseChannel::R_INV: 
            app_canTx_VC_RearInvertersStatus_set(status);
            break;
        case EfuseChannel::RSM:
            app_canTx_VC_RSMStatus_set(status);
            break;
        case EfuseChannel::BMS:
            app_canTx_VC_BMSStatus_set(status);
            break;
        case EfuseChannel::DAM:
            app_canTx_VC_DAMStatus_set(status);
            break;
        case EfuseChannel::FRONT:
            app_canTx_VC_FrontStatus_set(status);
            break;
        case EfuseChannel::MISC:
            app_canTx_VC_MiscStatus_set(status);
            break;
        // case EfuseChannel::PUMP:
        //     app_canTx_VC_RearLeftPumpStatus_set(status);
        //     break;
        // case EfuseChannel::R_RAD:
        //     app_canTx_VC_RightRadiatorFanStatus_set(status);
        //     break;
        // }
    }

    static const void efuse_current_setter (EfuseChannel &channel, float current){
        switch(channel){
        case EfuseChannel::F_INV:
            app_canTx_VC_FrontInvertersCurrent_set(current);
            break;
        case EfuseChannel::R_INV: 
            app_canTx_VC_RearInvertersCurrent_set(current);
            break;
        case EfuseChannel::RSM:
            app_canTx_VC_RSMCurrent_set(current);
            break;
        case EfuseChannel::BMS:
            app_canTx_VC_BMSCurrent_set(current);
            break;
        case EfuseChannel::DAM:
            app_canTx_VC_DAMCurrent_set(current);
            break;
        case EfuseChannel::FRONT:
            app_canTx_VC_FrontCurrent_set(current);
            break;
        case EfuseChannel::MISC:
            app_canTx_VC_MiscStatus_set(current);
            break;

        // case EfuseChannel::PUMP:
        //     app_canTx_VC_RearLeftPumpCurrent_set(current);
        //     break;
        // case EfuseChannel::R_RAD:
        //     app_canTx_VC_RightRadiatorFanCurrent_set(current);
        //     break;
        }
}
// I need a  setter that is void but takes in a bool to set the CAN message for the efuse status. 

// efuse_enabled_can_setters[...] is an array
// each element is a pointer to a function
// that function returns void and takes a single argument (bool)
// const means the pointers in the array can’t be changed after initialization
// static means it has internal linkage (only visible in this .c/.cpp file) and lives for the whole program
// static void (*const efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
//     [EFUSE_CHANNEL_F_INV]   = app_canTx_VC_FrontInvertersStatus_set,
//     [EFUSE_CHANNEL_F_INV]     = app_canTx_VC_RSMStatus_set,
//     [EFUSE_CHANNEL_BMS]     = app_canTx_VC_BMSStatus_set,
//     [EFUSE_CHANNEL_R_INV]   = app_canTx_VC_RearInvertersStatus_set,
//     [EFUSE_CHANNEL_DAM]     = app_canTx_VC_DAMStatus_set,
//     [EFUSE_CHANNEL_FRONT]   = app_canTx_VC_FrontStatus_set,
//     [EFUSE_CHANNEL_RL_PUMP] = app_canTx_VC_RearLeftPumpStatus_set,
//     [EFUSE_CHANNEL_R_RAD]   = app_canTx_VC_RightRadiatorFanStatus_set
// };

// static void (*const efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
//     [EFUSE_CHANNEL_F_INV]   = app_canTx_VC_FrontInvertersCurrent_set,
//     [EFUSE_CHANNEL_RSM]     = app_canTx_VC_RSMCurrent_set,
//     [EFUSE_CHANNEL_BMS]     = app_canTx_VC_BMSCurrent_set,
//     [EFUSE_CHANNEL_R_INV]   = app_canTx_VC_RearInvertersCurrent_set,
//     [EFUSE_CHANNEL_DAM]     = app_canTx_VC_DAMCurrent_set,
//     [EFUSE_CHANNEL_FRONT]   = app_canTx_VC_FrontCurrent_set,
//     [EFUSE_CHANNEL_RL_PUMP] = app_canTx_VC_RearLeftPumpCurrent_set,
//     [EFUSE_CHANNEL_R_RAD]   = app_canTx_VC_RightRadiatorFanCurrent_set
// };

// void app_efuse_broadcast(void)
// {
//     // run through each efuse, and broadcast the channel status and current
//     for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse += 1)
//     {
//         const bool  enabled = io_loadswitch_isChannelEnabled(efuse_channels[efuse]);
//         const float current = io_loadswitch_getChannelCurrent(efuse_channels[efuse]);

//         assert(efuse_enabled_can_setters[efuse] != NULL);
//         efuse_enabled_can_setters[efuse](enabled);
//         assert(efuse_current_can_setters[efuse] != NULL);
//         efuse_current_can_setters[efuse](current);
//     }
// }
