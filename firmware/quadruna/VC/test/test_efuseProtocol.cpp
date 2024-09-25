#include "test_vcBaseStateMachineTest.h"

extern "C"
{
#include "app_canTx.h"
#include "app_powerManager.h"
}

class VCEfuseProtocolTest : public VcBaseStateMachineTest
{
};

TEST_F(VCEfuseProtocolTest, shdn_protocol_unsuccessful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_SHDN, 0.3);

    SetInitialState(app_initState_get());
    LetTimePass(9000);

    EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_SHDN, false), 3);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_SHDN), 3);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 3);
}

TEST_F(VCEfuseProtocolTest, shdn_protocol_successful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_SHDN, 0.3);

    VcBaseStateMachineTest::SetStateToDrive();
    SetInitialState(app_driveState_get());
    LetTimePass(4000);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_SHDN, GOOD_CURRENT);
    LetTimePass(1000);

    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_SHDN, false), 2);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_SHDN), 2);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 0);
}

TEST_F(VCEfuseProtocolTest, lv_protocol_unsuccessful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LV, 0);

    SetInitialState(app_initState_get());
    LetTimePass(3100);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LV, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LV), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 1);
}

TEST_F(VCEfuseProtocolTest, lv_protocol_successful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LV, 0);

    SetInitialState(app_initState_get());
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LV, GOOD_CURRENT);
    LetTimePass(3000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LV, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LV), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 0);
}

TEST_F(VCEfuseProtocolTest, pump_protocol_unsuccessful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_PUMP, 0.3);

    SetInitialState(app_initState_get());
    LetTimePass(9000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_PUMP, false), 3);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_PUMP), 3);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 3);
}

TEST_F(VCEfuseProtocolTest, fans_protocol_successful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_PUMP, 0.3);

    SetInitialState(app_initState_get());
    LetTimePass(3000);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_PUMP, GOOD_CURRENT);
    LetTimePass(6000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_PUMP, false), 2);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_PUMP), 2);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 0);
}

TEST_F(VCEfuseProtocolTest, inv_r_protocol_unsuccessful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_R, 0.3);

    VcBaseStateMachineTest::SetStateToDrive();
    LetTimePass(3500);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_INV_R, false), 3);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_INV_R), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 1);
}

TEST_F(VCEfuseProtocolTest, inv_r_protocol_successful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_R, 0.3);

    VcBaseStateMachineTest::SetStateToDrive();
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_R, GOOD_CURRENT);
    LetTimePass(1000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_INV_R, false), 2);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_INV_R), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 0);
}

TEST_F(VCEfuseProtocolTest, inv_l_protocol_unsuccessful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_L, 0.3);

    VcBaseStateMachineTest::SetStateToDrive();
    LetTimePass(3500);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_INV_L, false), 3);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_INV_L), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 1);
}

TEST_F(VCEfuseProtocolTest, inv_l_protocol_successful)
{
    configEfuseChannels();
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_L, 0.3);

    VcBaseStateMachineTest::SetStateToDrive();
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_INV_L, GOOD_CURRENT);
    LetTimePass(1000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_INV_L, false), 2);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_INV_L), 1);
    EXPECT_EQ(app_canTx_VC_EFuseFaultCount_get(), 0);
}
