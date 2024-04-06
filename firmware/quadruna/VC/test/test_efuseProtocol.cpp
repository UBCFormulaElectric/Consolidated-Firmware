#include "test_vcBaseStateMachineTest.h"

class VCEfuseProtocolTest : public VcBaseStateMachineTest {
};

const int GOOD_CURRENT = 10;

TEST_F(VCEfuseProtocolTest, shdn_protocol_unsuccessful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_SHDN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LV, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_PUMP, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_TELEM, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_BUZZER, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_INV_L, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_INV_R, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_SHDN, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LV, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_PUMP, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_TELEM, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_BUZZER, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_L, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_R, GOOD_CURRENT);

    SetInitialState(app_initState_get());
    LetTimePass(2000);

    EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_SHDN, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_SHDN), 1);
}

// TEST_F(PdmEfuseProtocolTest, air_protocol_successful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(1250);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     LetTimePass(3000);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 0);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, true), 2);
// }

// TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_unsuccessful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(2500);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 0);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, true), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 1);
// }

// TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_successful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(1250);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     LetTimePass(3000);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 0);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, true), 2);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 2);
// }

// TEST_F(PdmEfuseProtocolTest, fans_protocol_unsuccessful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(6500);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, false), 3);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_FAN), 3);
// }

// TEST_F(PdmEfuseProtocolTest, fans_protocol_successful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(1250);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     LetTimePass(3000);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_FAN), 1);
// }

// TEST_F(PdmEfuseProtocolTest, inverters_protocol_unsuccessful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(2500);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 0);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, true), 1);
// }

// TEST_F(PdmEfuseProtocolTest, inverters_protocol_successful)
// {
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
//     fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 0.3);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

//     SetInitialState(app_driveState_get());
//     LetTimePass(1250);
//     fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
//     LetTimePass(3000);

//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 1);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
//     EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 0);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 2);
//     EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, true), 2);
// }