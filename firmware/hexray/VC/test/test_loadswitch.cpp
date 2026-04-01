#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_loadswitches.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_timeFake.hpp"

class VCLoadSwitchTest : public VCBaseTest
{
};

// test braodcasting of channels
TEST_F(VCLoadSwitchTest, load_switch_broadcast)
{
    efuse_channel_setters[0].can_setter_enabled(true);
    efuse_channel_setters[0].can_setter_current(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(efuse_channel_setters[0].efuse.isChannelEnabled());
    ASSERT_FLOAT_EQ(1.23f, efuse_channel_setters[0].efuse.getChannelCurrent());
    letTimePass(1);
    efuse_channel_setters[0].can_setter_enabled(false);
    efuse_channel_setters[0].can_setter_current(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(efuse_channel_setters[0].efuse.isChannelEnabled());
    ASSERT_FLOAT_EQ(0.23f, efuse_channel_setters[0].efuse.getChannelCurrent());
}
