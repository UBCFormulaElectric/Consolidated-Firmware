#include <gtest/gtest.h>
#include "test_pdmBaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_lowVoltageBattery.hpp"
#include "fake_io_efuse.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "app_powerManager.h"
}

const int GOOD_CURRENT = 10;

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class PdmEfuseProtocolTest : public PdmBaseStateMachineTest
{
};

TEST_F(PdmEfuseProtocolTest, init_state_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);

    SetInitialState(app_initState_get());
    LetTimePass(2500);

    EXPECT_EQ(PDM_INIT_STATE, app_canTx_PDM_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 0);
}

TEST_F(PdmEfuseProtocolTest, drive_state_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(2500);

    EXPECT_EQ(PDM_DRIVE_STATE, app_canTx_PDM_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, false), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 0);
}

TEST_F(PdmEfuseProtocolTest, air_protocol_unsuccessful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_EMETER, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_EMETER, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(2500);

    EXPECT_EQ(PDM_INIT_STATE, app_canTx_PDM_State_get());
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, true), 1);
}

TEST_F(PdmEfuseProtocolTest, air_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    LetTimePass(3000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, true), 2);
}

TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_unsuccessful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(2500);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, true), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 1);
}

TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    LetTimePass(3000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_LVPWR), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, true), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 2);
}

TEST_F(PdmEfuseProtocolTest, fans_protocol_unsuccessful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(6500);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, false), 3);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_FAN), 3);
}

TEST_F(PdmEfuseProtocolTest, fans_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    LetTimePass(3000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_FAN), 1);
}

TEST_F(PdmEfuseProtocolTest, inverters_protocol_unsuccessful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(2500);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, true), 1);
}

TEST_F(PdmEfuseProtocolTest, inverters_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, GOOD_CURRENT);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 0.3);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, GOOD_CURRENT);

    SetInitialState(app_driveState_get());
    LetTimePass(1250);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, GOOD_CURRENT);
    LetTimePass(3000);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, false), 1);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, false), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_LHS), 1);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_DI_RHS), 0);
    EXPECT_EQ(fake_io_efuse_standbyReset_callCountForArgs(EFUSE_CHANNEL_AIR), 0);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_RHS, true), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, true), 2);
}
