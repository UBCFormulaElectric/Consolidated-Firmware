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

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class PdmEfuseProtocolTest : public PdmBaseStateMachineTest
{
};

TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_successful)
{
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);

    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, 1);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 1);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 1);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 1);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, 1);

    SetInitialState(app_driveState_get());
    LetTimePass(150);

    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AIR, true), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_LVPWR, true), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_EMETER, false), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_AUX, false), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DRS, false), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_FAN, true), 2);
    EXPECT_EQ(fake_io_efuse_setChannel_callCountForArgs(EFUSE_CHANNEL_DI_LHS, true), 2);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_DI_RHS), true);
}
