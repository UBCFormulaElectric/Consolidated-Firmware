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
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_AIR, 0.7);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.7);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_FAN, 0.7);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_LHS, 0.7);
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_DI_RHS, 0.7);

    // fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_AIR, true);
    // fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_LVPWR, true);
    // fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_FAN, true);
    // fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_LHS, true);
    // fake_io_efuse_isChannelEnabled_returnsForArgs(EFUSE_CHANNEL_DI_RHS, true);
    
    SetInitialState(app_driveState_get());

    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_AIR), true);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_LVPWR), true);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_EMETER), false);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_AUX), false);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_DRS), false);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_FAN), true);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_DI_LHS), true);
    EXPECT_EQ(io_efuse_isChannelEnabled(EFUSE_CHANNEL_DI_RHS), true);
}

TEST_F(PdmEfuseProtocolTest, lvpwr_protocol_unseccessful) {}
